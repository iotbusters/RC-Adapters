#include <Assert.h>
#include <I2c.h>
#include <Utils.h>
#include <Wire.h>
#include <memory.h>

bool I2cClient::ensureState(const MasterSlaveState state) const {
    if (this->state == DISABLED)
        return false;

    guard(this->state == MASTER);
    return true;
}

void I2cClient::masterBegin() {
    this->state = DISABLED;

    auto wire = this->getWire();
    wire->begin();            // connecting the master to I2C peripherals.
    wire->onReceive(nullptr); // remove callback.
    wire->onRequest(nullptr); // remove callback.

    this->state = MASTER;
}

uint8_t I2cClient::masterWrite(const uint8_t slaveAddress, const uint8_t *bytes, const uint8_t size) const {
    if (!this->ensureState(MASTER))
        return 0;

    wire->beginTransmission(slaveAddress);
    auto count = wire->write(bytes, size);
    wire->endTransmission();
    return count;
}

uint8_t I2cClient::masterRead(const uint8_t slaveAddress, uint8_t *bytes, const uint8_t size) {
    if (!this->ensureState(MASTER))
        return 0;

    auto wire = this->getWire();
    wire->requestFrom(slaveAddress, size);
    return wire->readBytes(bytes, size);
}

void I2cClient::slaveBegin(const uint8_t slaveAddress, void (*onReceive)(int), void (*onRequest)()) {
    this->state = DISABLED;

    auto wire = this->getWire();
    wire->begin(slaveAddress); // connecting the I2C peripheral to master.
    wire->onReceive(onReceive);
    wire->onRequest(onRequest);

    this->state = SLAVE;
}

uint8_t I2cClient::slaveWrite(const uint8_t *bytes, const uint8_t size) {
    return this->ensureState(SLAVE) ? this->wire->write(bytes, size) : 0;
}

void I2cClient::disable() {
    this->state = DISABLED;
    this->getWire()->end();
}

void I2cMaster::begin() const { this->client->masterBegin(); }

uint8_t I2cMaster::write(const uint8_t slaveAddress, const uint8_t *bytes, const uint8_t size) const {
    return this->client->masterWrite(slaveAddress, bytes, size);
}

uint8_t I2cMaster::read(const uint8_t slaveAddress, uint8_t *bytes, const uint8_t size) {
    return this->client->masterRead(slaveAddress, bytes, size);
}

void I2cSlave::begin(uint8_t slaveAddress) { this->client->slaveBegin(slaveAddress, I2cSlave::onReceive, I2cSlave::onRequest); }

void I2cSlave::onRequest() {
    if (!I2cSlave::onMasterRequestAction)
        return;

    auto writer = I2cSlave::Writer(I2cSlave::client);
    I2cSlave::onMasterRequestAction(&writer);
}

void I2cSlave::onReceive(int bytesNumber) {
    guard(bytesNumber <= BUFFER_LENGTH);

    if (!I2cSlave::onMasterReceiveAction) {
        I2cSlave::client->slaveRead();
        return;
    }

    const uint8_t size = bytesNumber;
    uint8_t buffer[size];
    I2cSlave::client->slaveRead(buffer, size);

    auto reader = I2cSlave::Reader(buffer, size);
    I2cSlave::onMasterReceiveAction(&reader);
}

void I2cSlave::onMasterRequest(void (*onMasterRequest)(Writer *)) { this->onMasterRequestAction = onMasterRequest; }

template <typename T>
void I2cSlave::onMasterRequest(void (*onMasterRequest)(T, Writer *), T obj) {
    this->onMasterRequestAction = [onMasterRequest, obj](Writer *writer) { onMasterRequest(obj, writer); };
}

void I2cSlave::onMasterReceive(void (*onMasterReceive)(Reader *)) { this->onMasterReceiveAction = onMasterReceive; }

template <typename T>
void I2cSlave::onMasterReceive(void (*onMasterReceive)(T, Reader *), T obj) {
    this->onMasterRequestAction = [onMasterReceive, obj](Reader *reader) { onMasterReceive(obj, reader); };
}

uint8_t I2cSlave::Writer::write(const uint8_t *bytes, const uint8_t size) { return this->client->slaveWrite(bytes, size); }

template <uint8_t size>
uint8_t I2cSlave::Writer::write(const uint8_t (&bytes)[size]) {
    return this->write(bytes, size);
}

template <typename T>
uint8_t I2cSlave::Writer::write(const T value) {
    uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    uint8_t size = sizeof(T);
    return this->write(bytes, size);
}

uint8_t I2cSlave::Reader::read(uint8_t *bytes, const uint8_t size) {
    if (this->size == 0)
        return 0;

    auto availableSize = min(this->size, size);
    memcpy(bytes, &this->buffer[this->cursor], availableSize);

    this->cursor += availableSize;
    this->size -= availableSize;

    return availableSize;
}

template <typename T>
uint8_t I2cSlave::Reader::read(T &value) {
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&value);
    return this->read(bytes, sizeof(T));
}

// client will be initialized later
I2cClient *I2cMaster::client = nullptr;
I2cClient *I2cSlave::client = nullptr;
