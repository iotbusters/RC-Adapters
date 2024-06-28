#include <arduino.h>

struct ControllerOutput
{
    static ControllerOutput idle;

    bool speed2;     // speed raised to 2nd gear
    bool speed3;     // speed raised to 3nd gear
    float throttle;  // throttle value [0..1]
    bool isReversed; // reverse

    ControllerOutput();
    ControllerOutput(bool speed2, bool speed3, float throttle, bool isReversed);

    bool Breaks();

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;
};
