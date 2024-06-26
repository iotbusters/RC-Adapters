#include <arduino.h>

struct OutputData
{
    static OutputData idle;

    bool speed2;     // speed raised to 2nd gear
    bool speed3;     // speed raised to 3nd gear
    float throttle;  // throttle value [0..1]
    bool isReversed; // reverse

    OutputData();
    OutputData(bool speed2, bool speed3, float throttle, bool isReversed);

    bool Breaks();

    bool operator==(const OutputData &other) const;
    bool operator!=(const OutputData &other) const;
};
