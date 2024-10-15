#include <I2C.h>
#include <Assert.h>
#include <Wire.h>
#include <Utils.h>

void I2c::writeInteger12Bit(const int address, const int value12Bit) const
{
    assert(value12Bit >= INT_12BIT_MIN);
    assert(value12Bit <= INT_12BIT_MAX);

    const byte bytes[] = {
        (byte)(value12Bit >> 8),    // high 4 bits of the 12-bit value
        (byte)(value12Bit & 0xFF)}; // low 8 bits of the 12-bit value
    this->writeBytes(address, bytes);
}

void I2c::writeByte(const int address, const byte value) const
{
    this->writeBytes(address, {value});
}

template <size_t size>
void I2c::writeBytes(const int address, const byte (&data)[size]) const
{
    this->wire.beginTransmission(address);
    this->wire.write(data, size);
    this->wire.endTransmission();
}
