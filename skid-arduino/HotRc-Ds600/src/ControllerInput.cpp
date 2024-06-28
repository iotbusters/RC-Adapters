#include <ControllerInput.h>

ControllerInput::ControllerInput() : currentThrottle(0), desiredThrottle(0), steering(0) {}

ControllerInput::ControllerInput(float currentThrottle, float desiredThrottle, float steering)
{
    this->currentThrottle = currentThrottle;
    this->desiredThrottle = desiredThrottle;
    this->steering = steering;
}

ControllerInput ControllerInput::idle;
