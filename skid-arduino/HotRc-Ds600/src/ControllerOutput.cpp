#include <ControllerOutput.h>

ControllerOutput::ControllerOutput() : speed(0), throttle(0.0), isReversed(false) {}

ControllerOutput::ControllerOutput(byte speed, float throttle, bool isReversed)
{
    this->speed = speed;
    this->throttle = throttle;
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