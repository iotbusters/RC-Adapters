#include <inputdata.h>

InputData::InputData() : currentThrottle(0), desiredThrottle(0), steering(0) {}

InputData::InputData(float currentThrottle, float desiredThrottle, float steering)
{
    this->currentThrottle = currentThrottle;
    this->desiredThrottle = desiredThrottle;
    this->steering = steering;
}

InputData InputData::idle;
