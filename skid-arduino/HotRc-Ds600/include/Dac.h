#pragma once

#include <Arduino.h>
#include <I2C.h>

// MCP4725 DAC board addresses [0x60, 0x61] (L/H or open/close respectively)
// other model board addresses [0x62, 0x63]
#define I2C_ADR_THROTTLE_L 0x60
#define I2C_ADR_THROTTLE_R 0x61

#define THROTTLE_DESIRE_MIN 862  // 0.85v      / 4.04v(5v arduino) * INT_12BIT_MAX
#define THROTTLE_DESIRE_MAX 4000 // 3.27v +20% / 4.04v(5v arduino) * INT_12BIT_MAX

class Dac
{
private:
    I2c &i2c;
    byte address;

public:
    Dac(I2c &i2c, byte address) : i2c(i2c), address(address) {}
    void begin();

    void write(float throttle);
    void write(int value12Bit);
};
