#pragma once

#include <ControllerInput.h>
#include <ControllerOutput.h>

#define EPSILON ufract_t(0.01f)      // 1% or less is considered as a measurement error.
#define STEERING_MULT ufract_t(0.4f) // 40% of steering value is deducted from turning side throttle.

#define THROTTLE_REV SPEED_1_MAX    // 30% constant reverse throttle.
#define THROTTLE_ACC ufract_t(0.2f) // 20% throttle acceleration per second.

// throttle acceleration per second
#define THROTTLE_ACC_INC(timeDelta) min(ufract_t((float)timeDelta / 1000.0f) * THROTTLE_ACC, THROTTLE_ACC)

struct Controller {
private:
    const signed char turnSign;

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
