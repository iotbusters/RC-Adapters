#pragma once

#include <Arduino.h>
#include <Pins.h>
#include <RxOutput.h>


#define DS600RX_STEERING_MIN 551     // PWM min value of steering control
#define DS600RX_STEERING_MAX 2530    // PWM max value of steering control
#define DS600RX_THROTTLE_MIN 1090    // PWM min value of throttle control
#define DS600RX_THROTTLE_MAX 2075    // PWM min value of throttle control
#define DS600RX_ARMING_MIN 500       // PWM min value of arming control
#define DS600RX_ARMING_MAX 2500      // PWM max value of arming control
#define DS600RX_CALIBRATING_MIN 500  // PWM min value of calibrating control
#define DS600RX_CALIBRATING_MAX 2500 // PWM max value of calibrating control

#define DS900RX_AUX_MIN 2000 // PWM min value of auxiliary control

#define DS600RX_VALUE_EPSILON 0.05 // 5% or more is considered as a signal measurement error

#define CH_VALUE_PREV 0 // previous received channel index
#define CH_VALUE_CURR 1 // currently accepted channel index

#define CH_COUNT 4
#define CH_STEERING 0    // steering channel index
#define CH_THROTTLE 1    // throttle channel index
#define CH_ARMING 2      // arming channel index
#define CH_CALIBRATING 3 // calibrating channel index

#define LIM_MIN 0 // min limit
#define LIM_MAX 1 // max limit

class Ds600Rx {
private:
    const byte channelPins[CH_COUNT];
    // 4 channels with {previous, latest} values
    uint16_t channels[CH_COUNT][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
    bool linked = false;
    bool armed = false;

    bool isConnected() const;
    float getSteering() const;
    float getThrottle() const;
    bool isArmed() const;
    bool isCalibrating() const;
    // bool channel4();

public:
    Ds600Rx(const byte steeringPin, const byte throttlePin, const byte armingPin, const byte calibratingPin)
        : channelPins{steeringPin, throttlePin, armingPin, calibratingPin} {}

    void begin() const;

    const bool tryRead();
    RxOutput getOutput() const;
};
