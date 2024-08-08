#include <arduino.h>

#define PIN_D2 2
#define PIN_D3 3
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_D8 8
#define PIN_D9 9
#define PIN_D10 10
#define PIN_D11 11
#define PIN_D12 12
#define PIN_D13 13

#define CH1 PIN_D2 // RC channel 1 (steering) pin
#define CH2 PIN_D3 // RC channel 2 (throttle) pin
#define CH3 PIN_D4 // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5 // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6 // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7 // RC channel 6 (auxiliary) pin

// note: Free pins [D14..D19]

#define THROTTLE_CUR_L1 A0 // 1st left drive current throttle pin
#define THROTTLE_CUR_L2 A1 // 2nd left drive current throttle pin
#define THROTTLE_CUR_L3 A2 // 3rd left drive current throttle pin
#define THROTTLE_CUR_R1 A3 // 1st right drive current throttle pin
// note: A4 and A5 are dedicated to I2C
// I2C(SDA) : A4
// I2C(SCL) : A5
#define THROTTLE_CUR_R2 A6 // 2nd right drive current throttle pin
#define THROTTLE_CUR_R3 A7 // 3rd right drive current throttle pin

#define INT_12BIT_MIN 0
#define INT_12BIT_MAX 4095
#define THROTTLE_DESIRE_MIN 862  // 0.85v      / 4.04v(5v arduino) * INT_12BIT_MAX
#define THROTTLE_DESIRE_MAX 4000 // 3.27v +20% / 4.04v(5v arduino) * INT_12BIT_MAX

#define INT_10BIT_MIN 0
#define INT_10BIT_MAX 1023
#define THROTTLE_CURRENT_MIN 190           // pull-down resistor 6.8kOhms
#define THROTTLE_CURRENT_MAX INT_10BIT_MAX // theoretical, measurement is required

#define STEERING_MIN 553                                   // PWM min value of steering control
#define STEERING_MAX 2541                                  // PWM max value of steering control
#define STEERING_DEFAULT (STEERING_MIN + STEERING_MAX) / 2 // PWM middle value of steering control

#define THROTTLE_MIN 1100                                  // PWM min value of throttle control
#define THROTTLE_MAX 2092                                  // max min value of throttle control
#define THROTTLE_DEFAULT (THROTTLE_MIN + THROTTLE_MAX) / 2 // PWM middle value of throttle control

// MCP4725 DAC board addresses [0x60, 0x61] (L/H or open/close respectively)
// other model board addresses [0x62, 0x63]
#define I2C_ADR_THROTTLE_L 0x60
#define I2C_ADR_THROTTLE_R 0x61

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
#define I2C_RELAY_BREAKS 0    // Breaks pin on a 8bit IO expansion board
#define I2C_RELAY_REVERSE_L 1 // Left drive reverse pin on a 8bit IO expansion board
#define I2C_RELAY_REVERSE_R 2 // Right drive reverse pin on a 8bit IO expansion board
#define I2C_RELAY_SPEED2_L 3  // Left drive 2nd speed pin on a 8bit IO expansion board
#define I2C_RELAY_SPEED2_R 4  // Right drive 2nd speed pin on a 8bit IO expansion board
