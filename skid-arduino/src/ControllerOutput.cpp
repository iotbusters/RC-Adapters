#include <ControllerOutput.h>
#include <Utils.h>

byte getSpeedLevel(float throttle)
{
    for (byte speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;

    return 1; // unknown scenario
}

ControllerOutput::ControllerOutput(float throttle, bool reversed) : throttle(throttle), reversed(reversed)
{
    this->speed = getSpeedLevel(throttle);                                                                                     // find speed level by the throttle
    this->speedThrottle = mapNumber(throttle, maxSpeeds[this->speed - 1], maxSpeeds[this->speed], THROTTLE_MIN, THROTTLE_MAX); // map proportionally the controller throttle to selected speed throttle [10-100%]
}

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

const ControllerOutput ControllerOutput::idle(0.0, false);