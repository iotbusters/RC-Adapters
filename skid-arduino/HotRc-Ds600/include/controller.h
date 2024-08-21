#include <ControllerInput.h>
#include <ControllerOutput.h>

#define EPSILON 0.015f     // 1.5% or less is considered as a measurement error
#define STEERING_MULT 0.4f // 40% of steering value is deducted from turning side throttle

#define THROTTLE_ACC 0.2f                                               // 20% throttle acceleration
#define THROTTLE_ACC_INC(timeDelta) (timeDelta / 1000.0) * THROTTLE_ACC // throttle acceleration per second

struct Controller
{
private:
    char turnSign;
    int lastTime;
    ControllerInput request = ControllerInput::idle;
    ControllerOutput response = ControllerOutput::idle;

public:
    Controller(bool isLeft);

    bool tryUpdate(ControllerInput request);
    ControllerOutput getDrive();
};
