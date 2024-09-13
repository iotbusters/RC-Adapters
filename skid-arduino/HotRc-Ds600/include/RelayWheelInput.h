#include <Arduino.h>

struct RelayWheelInput
{
public:
    RelayWheelInput(bool reversed, bool lowSpeed) : reversed(reversed), lowSpeed(lowSpeed) {}

    bool reversed;
    bool lowSpeed;
};
