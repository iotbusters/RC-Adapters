#include <I2C.h>
#include <Assert.h>
#include <Wire.h>
#include <Utils.h>

static bool ensureState(MasterSlaveState state);

void I2cMaster::begin() const
{
    this->state = DISABLED;

    this->wire.begin();            // connecting the master to I2C peripherals.
    this->wire.onReceive(nullptr); // remove callback.
    this->wire.onRequest(nullptr); // remove callback.

    this->state = MASTER;
}

uint8_t I2cMaster::write(const uint8_t slaveAddress, const uint8_t *bytes, uint8_t size) const
{
    if (!ensureState(MASTER))
        return 0;

    this->wire.beginTransmission(slaveAddress);
    const auto count = this->wire.write(bytes, size);
    this->wire.endTransmission();

    return count;
}

template <uint8_t size>
uint8_t I2cMaster::write(const uint8_t slaveAddress, const uint8_t (&bytes)[size]) const
{
    return this->write(slaveAddress, bytes, size);
}

template <typename T>
uint8_t I2cMaster::write(const uint8_t slaveAddress, const T value) const
{
    uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    return this->write(slaveAddress, bytes, sizeof(T));
}

uint8_t I2cMaster::read(const uint8_t slaveAddress, uint8_t *bytes, uint8_t size)
{
    if (!ensureState(MASTER))
        return 0;

    this->wire.requestFrom(slaveAddress, size);
    return this->wire.readBytes(bytes, size);
}

template <uint8_t size>
uint8_t I2cMaster::read(const uint8_t slaveAddress, uint8_t (&bytes)[size])
{
    return this->read(slaveAddress, bytes);
}

template <typename T>
uint8_t I2cMaster::read(const uint8_t slaveAddress, T &value)
{
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&value);
    return this->read(slaveAddress, bytes, sizeof(T));
}

void I2cSlave::begin(uint8_t slaveAddress) const
{
    this->state = DISABLED;

    this->wire.begin(slaveAddress); // connecting the I2C peripheral to master.
    this->wire.onReceive(I2cSlave::onReceive);
    this->wire.onRequest(I2cSlave::onRequest);

    this->state = SLAVE;
}

void I2cSlave::onRequest(void (*onMasterRequest)(Writer &))
{
    this->onMasterRequest = onMasterRequest;
    this->onMasterRequestObject = nullptr;
}

template <typename T>
void I2cSlave::onRequest(void (*onMasterRequest)(Writer &, T &), T &obj)
{
    this->onMasterRequest = reinterpret_cast<void (*)(Writer &, void *)>(onMasterRequest);
    this->onMasterRequestObject = obj;
}

void I2cSlave::onReceive(void (*onMasterReceive)(Reader &))
{
    this->onMasterReceive = onMasterReceive;
    this->onMasterReceiveObject = nullptr;
}

template <typename T>
void I2cSlave::onReceive(void (*onMasterReceive)(Reader &, T &), T &obj)
{
    this->onMasterReceive = reinterpret_cast<void (*)(Reader &, void *)>(onMasterReceive);
    this->onMasterReceiveObject = obj;
}

void I2cSlave::onRequest()
{
    if (!ensureState(SLAVE))
        return;
    if (!I2cSlave::onMasterRequest)
        return;

    I2cSlave::onMasterRequest(I2cSlave::Writer(), I2cSlave::onMasterRequestObject);
}

void I2cSlave::onReceive(int bytesNumber)
{
    if (!ensureState(SLAVE))
        return;

    assert(bytesNumber <= BUFFER_LENGTH);

    if (!I2cSlave::onMasterReceive)
    {
        for (int i = 0; i < bytesNumber; i++)
            this->wire.read(); // clean the Wire buffer.
        return;
    }

    uint8_t buffer[BUFFER_LENGTH];
    for (int i = 0; i < bytesNumber; i++)
        buffer[i] = this->wire.read();

    auto reader = I2cSlave::Reader(&buffer, bytesNumber);
    I2cSlave::onMasterReceive(reader, I2cSlave::onMasterReceiveObject);
}

uint8_t I2cSlave::Writer::write(const uint8_t *bytes, uint8_t size)
{
    return this->wire.write(bytes, size);
}

template <uint8_t size>
uint8_t I2cSlave::Writer::write(const uint8_t (&bytes)[size])
{
    return this->write(bytes, size);
}

template <typename T>
uint8_t I2cSlave::Writer::write(const T value)
{
    uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    return this->write(bytes, sizeof(T));
}

uint8_t I2cSlave::Reader::read(uint8_t *bytes, uint8_t size)
{
    if (this->size == 0)
        return 0;

    auto availableSize = min(this->size, size);
    memcpy(bytes, &this->buffer[this->cursor], availableSize);

    this->cursor += availableSize;
    this->size -= availableSize;

    return availableSize;
}

template <uint8_t size>
uint8_t I2cSlave::Reader::read(uint8_t (&bytes)[size])
{
    return this->read(bytes, size);
}

template <typename T>
uint8_t I2cSlave::Reader::read(T &value)
{
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&value);
    return this->read(bytes, sizeof(T));
}

uint8_t I2cSlave::Reader::getSize() { return this -> size };

void I2c::disable()
{
    this->state = DISABLED;
    this->wire.end();
}

const I2c i2c;
TwoWire &I2c::wire = Wire;
MasterSlaveState I2c::state = DISABLED;
TwoWire &I2cSlave::wire = I2c::wire;
MasterSlaveState &I2cSlave::state = I2c::state;
TwoWire &I2cMaster::wire = I2c::wire;
MasterSlaveState &I2cMaster::state = I2c::state;

static bool ensureState(MasterSlaveState state)
{
    if (I2c::state == DISABLED)
        return false;

    assert(I2c::state == MASTER);

    return true;
}

// void appendOrSlide(const uint8_t *bytes, uint8_t size)
// {
//     auto maxSize = I2cSlave::reader.maxSize;
//     assert(size <= maxSize);

//     auto newSize = min(I2cSlave::reader.size + size, maxSize);

//     auto appendAtIndex = newSize - size;                      // an index to append into the buffer.
//     auto slidingSize = I2cSlave::reader.size - appendAtIndex; // a number of bytes in the buffer to slide to allow new bytes to be appended.

//     // slide the buffer if it's full.
//     if (slidingSize > 0)
//         memcpy(&I2cSlave::reader.buffer, &I2cSlave::reader.buffer[slidingSize], appendAtIndex);

//     // append new bytes.
//     memcpy(&I2cSlave::reader.buffer[appendAtIndex], bytes, size);
//     I2cSlave::reader.size = newSize;
// }

// void appendOrReset(const uint8_t *bytes, uint8_t size)
// {
//     auto maxSize = I2cSlave::reader.maxSize;
//     assert(size <= maxSize);

//     // reset the buffer if it's full.
//     if (I2cSlave::reader.size + size > maxSize)
//     {
//         if (I2cSlave::reader.size > size)
//             memset(&I2cSlave::reader.buffer[size], 0, I2cSlave::reader.size - size);
//         I2cSlave::reader.size = 0;
//     }

//     // append new bytes.
//     memcpy(&I2cSlave::reader.buffer[I2cSlave::reader.size], bytes, size);
//     I2cSlave::reader.size += size;
// }
