#pragma once

struct ControllerInput
{
public:
    static ControllerInput idle;

    float desiredThrottle; // desired throttle value [0..1]
    float steering;        // steering value [0..1]

    ControllerInput(float desiredThrottle, float steering) : desiredThrottle(desiredThrottle), steering(steering) {}
};
