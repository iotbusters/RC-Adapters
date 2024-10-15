#pragma once

#include <Arduino.h>
#include <Wire.h>

class I2c
{
private:
    TwoWire &wire;

public:
    explicit I2c(TwoWire &wire) : wire(wire) {}

    template <size_t size>
    void writeBytes(const int address, const byte (&data)[size]) const;

    void writeInteger12Bit(const int address, const int value12Bit) const;
    void writeByte(const int address, const byte value) const;
};
