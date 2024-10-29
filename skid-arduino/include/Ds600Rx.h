#pragma once

#include <Arduino.h>
#include <RxOutput.h>
#include <Pins.h>

#define CH1 PIN_D2 // RC channel 1 (steering) pin
#define CH2 PIN_D3 // RC channel 2 (throttle) pin
#define CH3 PIN_D4 // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5 // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6 // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7 // RC channel 6 (auxiliary) pin

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

#define CH_STEERING 0    // steering channel index
#define CH_THROTTLE 1    // throttle channel index
#define CH_ARMING 2      // arming channel index
#define CH_CALIBRATING 3 // calibrating channel index

#define LIM_MIN 0 // min limit
#define LIM_MAX 1 // max limit

class Ds600Rx
{
private:
    // 4 channels with {previous, latest} values
    uint16_t channels[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
    bool linked = false;
    bool armed = false;

    bool isConnected() const;
    float getSteering() const;
    float getThrottle() const;
    bool isArmed() const;
    bool isCalibrating() const;
    // bool channel4();

public:
    void begin() const;

    const bool tryRead();
    RxOutput getOutput() const;
};
