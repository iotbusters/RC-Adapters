#include <arduino.h>

struct ControllerOutput
{
    static ControllerOutput idle;

    byte speed;      // speed gear
    float throttle;  // throttle value [0..1]
    bool isReversed; // reverse

    ControllerOutput();
    ControllerOutput(byte speed, float throttle, bool isReversed);

    bool Breaks();

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;
};
