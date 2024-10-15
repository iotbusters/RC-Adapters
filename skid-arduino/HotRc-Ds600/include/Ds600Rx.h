#pragma once

#include <Arduino.h>
#include <Pins.h>

#define CH1 PIN_D2 // RC channel 1 (steering) pin
#define CH2 PIN_D3 // RC channel 2 (throttle) pin
#define CH3 PIN_D4 // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5 // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6 // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7 // RC channel 6 (auxiliary) pin

#define DS600RX_STEERING_MIN 553  // PWM min value of steering control
#define DS600RX_STEERING_MAX 2542 // PWM max value of steering control

#define DS600RX_THROTTLE_MIN 1101 // PWM min value of throttle control
#define DS600RX_THROTTLE_MAX 2093 // PWM min value of throttle control

class Ds600Rx
{
public:
    void begin();

    float channel1();
    float channel2();
    // float channel3();
    // float channel4();
    // float channel5();
    // float channel6();
};
