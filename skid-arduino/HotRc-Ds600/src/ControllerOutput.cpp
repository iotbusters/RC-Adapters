#include <ControllerOutput.h>

ControllerOutput::ControllerOutput() : speed2(false), speed3(false), throttle(0), isReversed(false) {}

ControllerOutput::ControllerOutput(bool speed2, bool speed3, float throttle, bool isReversed)
{
    this->speed2 = speed2;
    this->speed3 = speed3;
    this->throttle = throttle;
    this->isReversed = isReversed;
}

bool ControllerOutput::Breaks() { return *this == ControllerOutput::idle; }

bool ControllerOutput::operator==(const ControllerOutput &other) const
{
    return speed2 == other.speed2 && speed3 == other.speed3 && throttle == other.throttle && isReversed == other.isReversed;
}

bool ControllerOutput::operator!=(const ControllerOutput &other) const
{
    return !(*this == other);
}

ControllerOutput ControllerOutput::idle;