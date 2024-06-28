#include <ControllerInput.h>
#include <ControllerOutput.h>

#define SPEED_ACC_MULT 1.2f // 120%
#define SPEED_1_MAX 0.2f    // 20%
#define SPEED_2_MAX 0.5f    // 50%
#define SPEED_3_MAX 1.0f    // 100%
const float maxSpeeds[] = {0.0f, SPEED_1_MAX, SPEED_2_MAX, SPEED_3_MAX};

#define EPSILON 0.01f      // 1% or less is considered as a measurement error
#define STEERING_MULT 0.4f // 40% of steering value is reduced from turning side throttle

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