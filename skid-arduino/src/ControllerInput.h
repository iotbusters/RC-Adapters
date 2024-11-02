#pragma once

struct ControllerInput
{
public:
    static const ControllerInput idle;

    float steering;        // steering value [0..1]
    float desiredThrottle; // desired throttle value [0..1]

    ControllerInput(float steering, float desiredThrottle)
        : steering(steering), desiredThrottle(desiredThrottle) {}
};
