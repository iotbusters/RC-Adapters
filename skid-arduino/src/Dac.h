#pragma once

#include <Arduino.h>
#include <I2c.h>

// MCP4725 DAC board addresses [0x60, 0x61] (L/H or open/close respectively)
// other model board addresses [0x62, 0x63]
#define I2C_ADR_THROTTLE_L 0x60
#define I2C_ADR_THROTTLE_R 0x61

class Dac {
private:
    const I2cMaster &i2c;
    const uint8_t address;

public:
    Dac(const I2cMaster &i2c, const uint8_t address) : i2c(i2c), address(address) {}
    void begin() const;

    void write(const ufract_t value) const;
    void write(const uint12_t value) const;
};
