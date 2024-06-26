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
#define PIN_D14 A0
#define PIN_D15 A1
#define PIN_D16 A2
#define PIN_D17 A3
#define PIN_D18 A6
#define PIN_D19 A7

// I2C(SDA) PIN_A4
// I2C(SCL) PIN_A5
#define CH1 PIN_D2
#define CH2 PIN_D3
#define CH3 PIN_D4
#define CH4 PIN_D5
#define CH5 PIN_D6
#define CH6 PIN_D7
#define BREAKS PIN_D8
// D9
#define REVERSE_L PIN_D10
#define REVERSE_R PIN_D11
#define SPEED_L2 PIN_D12
#define SPEED_L3 PIN_D13
// #define SPEED_R2 PIN_D
// #define SPEED_R3 PIN_D
#define THROTTLE_CUR_L1 PIN_D14
#define THROTTLE_CUR_L2 PIN_D15
#define THROTTLE_CUR_L3 PIN_D16
#define THROTTLE_CUR_R1 PIN_D17
#define THROTTLE_CUR_R2 PIN_D18
#define THROTTLE_CUR_R3 PIN_D19

#define I2C_THROTTLE_LEFT 0x62
#define I2C_THROTTLE_RIGHT 0x63
