#pragma once

#include <Arduino.h>
#include <Arduino-CRSF.h>
#include <RxOutput.h>
#include <Pins.h>

#define ES900RX_STEERING_MIN 174  // ES900RX PWM min value of steering control
#define ES900RX_STEERING_MAX 1805 // ES900RX PWM max value of steering control

#define ES900RX_THROTTLE_MIN 174  // ES900RX PWM min value of throttle control
#define ES900RX_THROTTLE_MAX 1805 // ES900RX PWM max value of throttle control

#define ES900RX_AUX_MIN 1500 // ES900RX PWM min value of auxiliary control

class Es900Rx
{
private:
    HardwareSerial &logger;
    SoftwareSerial &crossfireSerial;
    CRSF crossfire;
    bool linked = false;
    bool armed = false;

    float getSteering() const;
    float getThrottle() const;
    // float channel3() const;
    // float channel4() const;
    bool isArmed() const;
    // float channel6() const;
    // float channel7() const;
    // float channel8() const;

public:
    Es900Rx(HardwareSerial &logger, SoftwareSerial &crossfireSerial)
        : logger(logger), crossfireSerial(crossfireSerial) {}

    void begin();

    const bool tryRead();
    RxOutput getOutput() const;
};
