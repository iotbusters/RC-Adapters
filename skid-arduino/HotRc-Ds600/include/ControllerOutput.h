#pragma once

#include <Arduino.h>

#define SPEED_STOP 0.0f
#define SPEED_2_MAX 0.3f // 30% of total throttle
#define SPEED_3_MAX 1.0f // 100% of total throttle
const float maxSpeeds[] = {SPEED_STOP, SPEED_2_MAX, SPEED_3_MAX};

#define THROTTLE_MIN 0.2f // 20% speed throttle to start moving with
#define THROTTLE_MAX 1.0f // 100% speed throttle

struct ControllerOutput
{
public:
    static ControllerOutput idle;

    float throttle;      // controller throttle value [0..1]
    byte speed;          // speed gear
    float speedThrottle; // speed related throttle value [0..1]
    bool reversed;       // reverse

    ControllerOutput(float throttle, bool reversed);

    bool breaks() const;

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;
};
