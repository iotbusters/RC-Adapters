#include <Arduino.h>

struct RelayWheelInput {
public:
    const bool reversed;
    const bool highSpeed;

    RelayWheelInput(const bool reversed, const bool highSpeed) : reversed(reversed), highSpeed(highSpeed) {}
};
