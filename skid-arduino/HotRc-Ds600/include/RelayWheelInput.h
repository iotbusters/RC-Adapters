#include <Arduino.h>

struct RelayWheelInput
{
public:
    const bool reversed;
    const bool lowSpeed;

    RelayWheelInput(bool reversed, bool lowSpeed) : reversed(reversed), lowSpeed(lowSpeed) {}
};
