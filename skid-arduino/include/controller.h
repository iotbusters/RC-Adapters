#pragma once

#include <Arduino.h>
#include <ControllerInput.h>
#include <ControllerOutput.h>

#define EPSILON 0.01f      // 1% or less is considered as a measurement error
#define STEERING_MULT 0.4f // 40% of steering value is deducted from turning side throttle

#define THROTTLE_REV SPEED_2_MAX // 30% constant reverse throttle
#define THROTTLE_ACC 0.2f        // 20% throttle acceleration

#define THROTTLE_ACC_INC(speed, timeDelta)                                                                                                 \
    min((speed == 1 ? 0.5f : 1.0f) * (timeDelta / 1000.0) * THROTTLE_ACC, THROTTLE_ACC) // throttle acceleration per second

struct Controller {
private:
    const char turnSign;

    int lastTime = millis();
    bool accelerating = false;
    ControllerInput input = ControllerInput::idle;
    ControllerOutput output = ControllerOutput::idle;

    bool isChanged(const ControllerInput &next);

public:
    explicit Controller(bool isLeft) : turnSign(isLeft ? -1 : 1){};

    bool tryUpdate(const ControllerInput &input);
    const ControllerOutput &getOutput() const;
};
