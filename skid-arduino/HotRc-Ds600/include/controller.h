#include <ControllerInput.h>
#include <ControllerOutput.h>

#define SPEED_ACC_MULT 1.2f // 120%
#define SPEED_STOP 0.0f
#define SPEED_2_MAX 0.4f // 40%
#define SPEED_3_MAX 1.0f // 100%
const float maxSpeeds[] = {SPEED_STOP, SPEED_2_MAX, SPEED_3_MAX};

#define EPSILON 0.01f      // 1% or less is considered as a measurement error
#define THROTTLE_MIN 0.1f  // 10% throttle to start moving with
#define STEERING_MULT 0.4f // 40% of steering value is deducted from turning side throttle

struct Controller
{
private:
    char turnSign;
    ControllerInput request = ControllerInput::idle;
    ControllerOutput response = ControllerOutput::idle;

public:
    Controller(bool isLeft);

    bool tryUpdate(ControllerInput request);
    ControllerOutput getDrive();
};