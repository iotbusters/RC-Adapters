#pragma once

#include <Arduino.h>
#include <Wire.h>

class I2c
{
private:
    TwoWire &wire;

public:
    I2c(TwoWire &wire) : wire(wire) {}

    void write(int address, const int value12Bit);
    void write(int address, const byte value);

    template <size_t size>
    void write(int address, const byte (&data)[size]);
};
