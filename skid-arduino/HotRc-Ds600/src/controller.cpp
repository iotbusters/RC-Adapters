#include <arduino.h>
#include <controller.h>
#include <tools.h>

uint8_t getSpeedLevel(float throttle)
{
    for (uint8_t speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;
    return 1; // first speed for unknown scenario
}

bool isChanged(InputData last, InputData current)
{
    return abs(last.currentThrottle - current.currentThrottle) < EPSILON    // no relevant current throttle change
           && abs(last.desiredThrottle - current.desiredThrottle) < EPSILON // no relevant desired throttle change
           && abs(last.steering - current.steering) < EPSILON;              // no relevant steering change
}

Controller::Controller(bool isLeft) { this->turnSign = isLeft ? -1 : 1; }

bool Controller::tryCalculate(InputData request, OutputData *response)
{
    if (isChanged(this->last, request))
    {
        *response = OutputData::idle;
        return false; // nothing to recalculate
    }

    this->last = request;

    float steering = abs(request.steering);
    float currentThrottle = abs(request.currentThrottle);
    float desiredThrottle = abs(request.desiredThrottle);

    if (steering < EPSILON && desiredThrottle < EPSILON && currentThrottle < EPSILON)
    {
        *response = OutputData::idle;
        return true; // got back to idle
    }

    bool isForward = request.desiredThrottle > EPSILON;
    bool isBackward = request.desiredThrottle < -EPSILON;
    bool isTurning = request.steering * this->turnSign > EPSILON;

    if (isForward)
    {
        // throttle strategies
        // [ ] proportional when throttle is directly mapped on speed scale [1..3]
        // [x] smoothly accelerated throttle up to requested value by increasing 20% current throttle to desired
        desiredThrottle = min(
            min(currentThrottle * SPEED_ACC_MULT, desiredThrottle), // smoothly accelerated throttle
            desiredThrottle);                                       // maximum (desired) throttle

        if (isTurning)
            desiredThrottle = max(desiredThrottle - steering * STEERING_MULT, EPSILON); // drop throttle down to turn

        uint8_t speed = getSpeedLevel(desiredThrottle);
        float throttle = fmap(desiredThrottle, maxSpeeds[speed - 1], maxSpeeds[speed], 0.0f, 1.0f);
        *response = OutputData(speed >= 2, speed == 3, throttle, false);
        return true;
    }

    if (isBackward)
    {
        float throttle = isTurning
                             ? 0        // no move
                             : EPSILON; // min throttle to activate reverse
        *response = OutputData(false, false, throttle, !isTurning);
        return true;
    }

    // spinning
    float throttle = isTurning
                         ? EPSILON      // min throttle to activate reverse
                         : SPEED_1_MAX; // equalize reverse constant speed
    *response = OutputData(false, false, throttle, isTurning);
    return true;
}

Controller leftController = Controller(true);
Controller rightController = Controller(false);