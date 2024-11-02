#include <ControllerOutput.h>
#include <Utils.h>

// find speed level by the throttle
byte getSpeedLevel(float throttle)
{
    for (byte speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;

    return 1; // unknown scenario
}

// map proportionally the controller throttle to selected speed throttle [10-100%]
float getSpeedThrottle(float throttle, int speed)
{
    return mapNumber(throttle, maxSpeeds[speed - 1], maxSpeeds[speed], THROTTLE_MIN, THROTTLE_MAX);
}

ControllerOutput::ControllerOutput(float throttle, bool reversed)
    : throttle(throttle), reversed(reversed), speed(getSpeedLevel(throttle)), highSpeed(this->speed == 2), speedThrottle(getSpeedThrottle(throttle, this->speed)) {}

bool ControllerOutput::breaks() const
{
    return *this == ControllerOutput::idle;
}

bool ControllerOutput::operator==(const ControllerOutput &other) const
{
    return speed == other.speed && throttle == other.throttle && reversed == other.reversed;
}

bool ControllerOutput::operator!=(const ControllerOutput &other) const
{
    return !(*this == other);
}

const ControllerOutput ControllerOutput::idle;