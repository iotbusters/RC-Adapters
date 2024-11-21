#include <ControllerOutput.h>

bool ControllerOutput::breaks() const { return *this == ControllerOutput::idle; }

bool ControllerOutput::operator==(const ControllerOutput &other) const {
    return desiredThrottle == other.desiredThrottle && reversed == other.reversed;
}

bool ControllerOutput::operator!=(const ControllerOutput &other) const { return !(*this == other); }

const ControllerOutput ControllerOutput::idle;