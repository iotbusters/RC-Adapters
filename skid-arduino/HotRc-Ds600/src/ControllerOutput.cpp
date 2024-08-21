#include <ControllerOutput.h>
#include <Utils.h>

byte getSpeedLevel(float throttle)
{
    for (byte speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;

    return 1; // unknown scenario
}

ControllerOutput::ControllerOutput() : throttle(0.0), speed(0), speedThrottle(0.0), isReversed(false) {}

ControllerOutput::ControllerOutput(float throttle, bool isReversed)
{
    this->throttle = throttle;
    // find speed level by the throttle
    this->speed = getSpeedLevel(throttle);
    // map proportionally the controller throttle to selected speed throttle [10-100%]
    this->speedThrottle = mapNumber(throttle, maxSpeeds[this->speed - 1], maxSpeeds[this->speed], THROTTLE_MIN, THROTTLE_MAX);
    this->isReversed = isReversed;
}

bool ControllerOutput::Breaks() { return *this == ControllerOutput::idle; }

bool ControllerOutput::operator==(const ControllerOutput &other) const
{
    return speed == other.speed && throttle == other.throttle && isReversed == other.isReversed;
}

bool ControllerOutput::operator!=(const ControllerOutput &other) const
{
    return !(*this == other);
}

ControllerOutput ControllerOutput::idle;