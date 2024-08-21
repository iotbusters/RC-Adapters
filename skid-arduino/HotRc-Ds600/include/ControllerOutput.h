#include <arduino.h>

#define SPEED_STOP 0.0f
#define SPEED_2_MAX 0.3f // 30%
#define SPEED_3_MAX 1.0f // 100%
const float maxSpeeds[] = {SPEED_STOP, SPEED_2_MAX, SPEED_3_MAX};

#define THROTTLE_MIN 0.1f // 10% throttle to start moving with
#define THROTTLE_MAX 1.0f // 100% throttle

struct ControllerOutput
{
    static ControllerOutput idle;

    float throttle;      // controller throttle value [0..1]
    byte speed;          // speed gear
    float speedThrottle; // speed related throttle value [0..1]
    bool isReversed;     // reverse

    ControllerOutput();
    ControllerOutput(float throttle, bool isReversed);

    bool Breaks();

    bool operator==(const ControllerOutput &other) const;
    bool operator!=(const ControllerOutput &other) const;
};
