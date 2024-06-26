#include <inputdata.h>
#include <outputdata.h>

#define INT_12BIT_MIN_VALUE 0
#define INT_12BIT_MAX_VALUE 4095

#define SPEED_MAX_VALUE 1023

#define STEERING_MIN_VALUE 560
#define STEERING_MAX_VALUE 2560
#define STEERING_MIDDLE_VALUE (STEERING_MAX_VALUE + STEERING_MIN_VALUE) / 2 // the point between left and right

#define THROTTLE_MIN_VALUE 1100
#define THROTTLE_MAX_VALUE 2100
#define THROTTLE_MIDDLE_VALUE (THROTTLE_MAX_VALUE + THROTTLE_MIN_VALUE) / 2 // the point between forward and reverse

#define SPEED_ACC_MULT 1.2f // 120%
#define SPEED_1_MAX 0.2f    // 20%
#define SPEED_2_MAX 0.5f    // 50%
#define SPEED_3_MAX 1.0f    // 100%
const float maxSpeeds[] = {0.0f, SPEED_1_MAX, SPEED_2_MAX, SPEED_3_MAX};

#define EPSILON 0.02f      // 2% or less is considered as a measurement error
#define STEERING_MULT 0.4f // 40% of steering value is reduced from turning side throttle

struct Controller
{
private:
    char turnSign;
    InputData last = InputData::idle;

public:
    Controller(bool isLeft);

    bool tryCalculate(InputData request, OutputData *response);
};