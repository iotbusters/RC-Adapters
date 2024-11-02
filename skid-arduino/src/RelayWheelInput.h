#include <Arduino.h>

struct RelayWheelInput
{
public:
    const bool reversed;
    const bool highSpeed;

    RelayWheelInput(bool reversed, bool highSpeed)
        : reversed(reversed), highSpeed(highSpeed) {}
};
