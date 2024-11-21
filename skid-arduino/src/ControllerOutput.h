#pragma once

#include <Utils.h>

#define SPEED_STOP UFRACT_MIN
#define SPEED_1_MAX ufract_t(0.3f) // 30% of total throttle.
#define SPEED_2_MAX UFRACT_MAX     // 100% of total throttle.

const ufract_t maxSpeeds[] = {SPEED_STOP, SPEED_1_MAX, SPEED_2_MAX};

struct ControllerOutput {
    // find speed level by the throttle.
    static uint8_t getSpeed(const ufract_t throttle) {
        for (uint8_t i = 1; i < size(maxSpeeds); i++)
            if (throttle <= maxSpeeds[i])
                return i;
        return 1; // unknown scenario.
    }
    // map proportionally the controller throttle to selected speed throttle.
    static ufract_t getSpeedThrottle(const ufract_t throttle, const int speed) {
        return mapNumber(throttle, maxSpeeds[speed - 1], maxSpeeds[speed], UFRACT_MIN, UFRACT_MAX);
    }

public:
    static const ControllerOutput idle;

    const uint8_t speed;            // high speed gear flag.
    const ufract_t speedThrottle;   // speed related throttle fraction.
    const ufract_t desiredThrottle; // desired throttle fraction.
    const bool reversed;            // reverse flag.
    const bool highSpeed;           // high speed gear flag.

    ControllerOutput(const ufract_t desiredThrottle, const bool reversed)
        : speed(getSpeed(desiredThrottle)), speedThrottle(getSpeedThrottle(desiredThrottle, this->speed)), desiredThrottle(desiredThrottle),
          reversed(reversed), highSpeed(this->speed == 2) {}

    bool breaks() const { return *this == ControllerOutput::idle; }
    void operator=(const ControllerOutput &value) const { value = ControllerOutput(desiredThrottle, reversed); }
    bool operator==(const ControllerOutput &other) const {
        return speed == other.speed && desiredThrottle == other.desiredThrottle && reversed == other.reversed;
    }
    bool operator!=(const ControllerOutput &other) const { return !(*this == other); }

private:
    ControllerOutput() : speed(1), speedThrottle(UFRACT_MIN), desiredThrottle(UFRACT_MIN), reversed(false), highSpeed(false) {}
};
