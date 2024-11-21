#pragma once

#include <Utils.h>

struct ControllerInput {
public:
    static const ControllerInput idle;

    const fract_t steering;        // steering ratio
    const fract_t desiredThrottle; // desired throttle ratio

    ControllerInput() : steering(FRACT_ZERO), desiredThrottle(FRACT_ZERO) {}
    ControllerInput(const fract_t steering, const fract_t desiredThrottle) : steering(steering), desiredThrottle(desiredThrottle) {}

    void operator=(const ControllerInput &value) const { value = ControllerInput(steering, desiredThrottle); }
};
