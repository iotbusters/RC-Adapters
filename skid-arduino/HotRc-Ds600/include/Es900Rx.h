#pragma once

#include <Arduino.h>
#include <Arduino-CRSF.h>
#include <RxOutput.h>
#include <Pins.h>

#define UART_CRSF_RX PIN_D8 // UART RX pin to TX pin on crossfire receiver
#define UART_CRSF_TX PIN_D9 // UART TX pin to RX pin on crossfire receiver

#define ES900RX_STEERING_MIN 174  // ES900RX PWM min value of steering control
#define ES900RX_STEERING_MAX 1805 // ES900RX PWM max value of steering control

#define ES900RX_THROTTLE_MIN 174  // ES900RX PWM min value of throttle control
#define ES900RX_THROTTLE_MAX 1805 // ES900RX PWM max value of throttle control

#define ES900RX_AUX_MIN 1500 // ES900RX PWM min value of auxiliary control

class Es900Rx
{
private:
    SoftwareSerial serial = SoftwareSerial(UART_CRSF_RX, UART_CRSF_TX);
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
    void begin();

    const bool tryRead();
    RxOutput getOutput() const;
};
