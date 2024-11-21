#pragma once

#include <Arduino.h>
#include <Utils.h>

#define SPEED_STOP 0.0f
#define SPEED_2_MAX 0.3f // 30% of total throttle
#define SPEED_3_MAX 1.0f // 100% of total throttle
const float maxSpeeds[] = {SPEED_STOP, SPEED_2_MAX, SPEED_3_MAX};

#define THROTTLE_MIN 0.10f // 10% speed throttle to start moving with
#define THROTTLE_MAX 1.0f  // 100% speed throttle

struct ControllerOutput {
private:
    // find speed level by the throttle
    byte getSpeedLevel(float throttle) {
        for (byte speed = 1; speed < size(maxSpeeds); speed++)
            if (throttle <= maxSpeeds[speed])
                return speed;

        return 1; // unknown scenario
    }
    // map proportionally the controller throttle to selected speed throttle
    float getSpeedThrottle(float desiredThrottle, int speed) {
        return mapNumber(desiredThrottle, maxSpeeds[speed - 1], maxSpeeds[speed], THROTTLE_MIN, THROTTLE_MAX);
    }

public:
    static const ControllerOutput idle;

    byte speed;            // high speed gear flag
    bool highSpeed;        // high speed gear flag
    float speedThrottle;   // speed related throttle value [0..1]
    float desiredThrottle; // controller throttle value [0..1]
    bool reversed;         // reverse flag

    ControllerOutput(float desiredThrottle, bool reversed)
        : speed(getSpeedLevel(desiredThrottle)), highSpeed(this->speed == 2), speedThrottle(getSpeedThrottle(desiredThrottle, this->speed)),
          desiredThrottle(desiredThrottle), reversed(reversed) {}

    bool breaks() const;

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;

private:
    ControllerOutput() : speed(1), highSpeed(false), speedThrottle(0.0), desiredThrottle(0.0), reversed(false) {}
};
