#pragma once

#include <Arduino.h>
#include <Utils.h>

#define SPEED_STOP 0.0f
#define SPEED_2_MAX 0.3f // 30% of total throttle
#define SPEED_3_MAX 1.0f // 100% of total throttle
const float maxSpeeds[] = {SPEED_STOP, SPEED_2_MAX, SPEED_3_MAX};

#define THROTTLE_MIN 0.0f // 0% speed throttle to start moving with
#define THROTTLE_MAX 1.0f // 100% speed throttle

struct ControllerOutput
{
private:
    bool speed; // high speed gear flag

    ControllerOutput()
        : speed(1), highSpeed(false), speedThrottle(0.0), throttle(0.0), reversed(false) {}

public:
    static const ControllerOutput idle;

    bool highSpeed;      // high speed gear flag
    float speedThrottle; // speed related throttle value [0..1]
    float throttle;      // controller throttle value [0..1]
    bool reversed;       // reverse flag

    ControllerOutput(float throttle, bool reversed);

    bool breaks() const;

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;
};
