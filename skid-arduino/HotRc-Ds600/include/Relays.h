#pragma once

#include <I2C.h>
#include <RelayWheelInput.h>

// PCF8574 IO expansion board addresses [0x20..0x27]
// | A2 | A1 | A0 | I2C address |
// | 0  | 0  | 0  | 0x20        |
// | 0  | 0  | 1  | 0x21        |
// | 0  | 1  | 0  | 0x22        |
// | 0  | 1  | 1  | 0x23        |
// | 1  | 0  | 0  | 0x24        |
// | 1  | 0  | 1  | 0x25        |
// | 1  | 1  | 0  | 0x26        |
// | 1  | 1  | 1  | 0x27        |
#define I2C_ADR_RELAYS 0x20
#define I2C_RELAY_SPEED2_L 0  // Left drive 2nd speed pin on a 8bit IO expansion board
#define I2C_RELAY_REVERSE_L 1 // Left drive reverse pin on a 8bit IO expansion board
#define I2C_RELAY_BREAKS 2    // Breaks pin on a 8bit IO expansion board
#define I2C_RELAY_REVERSE_R 3 // Right drive reverse pin on a 8bit IO expansion board
#define I2C_RELAY_SPEED2_R 4  // Right drive 2nd speed pin on a 8bit IO expansion board

class Relays
{
private:
    I2c &i2c;

public:
    Relays(I2c &i2c) : i2c(i2c) {}
    void begin();

    void write(bool breaks, RelayWheelInput left, RelayWheelInput right);
};
