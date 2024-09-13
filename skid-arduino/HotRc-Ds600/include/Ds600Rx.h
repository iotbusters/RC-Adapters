#pragma once

#include <Arduino.h>

#define SIG_STEERING_MIN 553  // PWM min value of steering control
#define SIG_STEERING_MAX 2541 // PWM max value of steering control

#define SIG_THROTTLE_MIN 1100 // PWM min value of throttle control
#define SIG_THROTTLE_MAX 2092 // max min value of throttle control

class Ds600Rx
{
public:
    static void begin();
    static float channel1();
    static float channel2();
    // static float channel3();
    // static float channel4();
    // static float channel5();
    // static float channel6();
};
