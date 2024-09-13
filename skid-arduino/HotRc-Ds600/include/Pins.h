#include <Arduino.h>

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
// note: Free pins [D14..D19]

#define CH1 PIN_D2 // RC channel 1 (steering) pin
#define CH2 PIN_D3 // RC channel 2 (throttle) pin
#define CH3 PIN_D4 // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5 // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6 // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7 // RC channel 6 (auxiliary) pin

#define UART_CRSF_RX PIN_D8 // UART RX pin for crossfire receiver
#define UART_CRSF_TX PIN_D9 // UART TX pin for crossfire receiver

#define THROTTLE_CUR_L1 PIN_A0 // 1st left drive current throttle pin
#define THROTTLE_CUR_L2 PIN_A1 // 2nd left drive current throttle pin
#define THROTTLE_CUR_L3 PIN_A2 // 3rd left drive current throttle pin
#define THROTTLE_CUR_R1 PIN_A3 // 1st right drive current throttle pin
// note: A4 and A5 are dedicated to I2C
// I2C(SDA) : A4
// I2C(SCL) : A5
#define THROTTLE_CUR_R2 PIN_A6 // 2nd right drive current throttle pin
#define THROTTLE_CUR_R3 PIN_A7 // 3rd right drive current throttle pin

const byte leftCurrentThrottlePins[] = {THROTTLE_CUR_L1, THROTTLE_CUR_L2, THROTTLE_CUR_L3};
const byte rightCurrentThrottlePins[] = {THROTTLE_CUR_R1, THROTTLE_CUR_R2, THROTTLE_CUR_R3};
