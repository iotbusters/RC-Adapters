#include <Arduino.h>
#include <Controller.h>
#include <Utils.h>

byte getSpeedLevel(float throttle)
{
    for (byte speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;
    return 1; // first speed for unknown scenario
}

bool isChanged(ControllerInput last, ControllerInput current)
{
    return current.desiredThrottle > current.currentThrottle                // throttle is being accelerated smoothly up to desired value
           || abs(last.desiredThrottle - current.desiredThrottle) > EPSILON // no relevant desired throttle change
           || abs(last.steering - current.steering) > EPSILON;              // no relevant steering change
}

Controller::Controller(bool isLeft) { this->turnSign = isLeft ? -1 : 1; }

bool Controller::tryUpdate(ControllerInput request)
{
    if (!isChanged(this->request, request))
        return false; // nothing to recalculate

    this->request = request;

    float steering = abs(request.steering);
    float currentThrottle = abs(request.currentThrottle);
    float desiredThrottle = abs(request.desiredThrottle);

    if (steering < EPSILON && desiredThrottle < EPSILON)
    {
        this->request = ControllerInput::idle;
        this->response = ControllerOutput::idle;
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
        float acceleratedThrottle = min(currentThrottle * SPEED_ACC_MULT, desiredThrottle); // smoothly accelerated throttle up to its desired value

        if (isTurning)
        {
            // impact accelerated throttle proportionally to its desired and steering values
            float turningFactor = (1.0f - steering * STEERING_MULT / desiredThrottle);
            // drop throttle down to turn
            acceleratedThrottle = max(acceleratedThrottle * turningFactor, EPSILON);
        }

        // find speed level by current throttle
        byte speed = getSpeedLevel(desiredThrottle);
        // map proportionally current throttle to selected speed throttle [10-100%]
        float speedScaledThrottle = mapNumber(acceleratedThrottle, max(0.0f, maxSpeeds[speed - 1] - 0.1f), maxSpeeds[speed], 0.0f, 1.0f);
        this->response = ControllerOutput(speed >= 2 /*speed2*/, speed == 3 /*speed3*/, speedScaledThrottle, false /*reverse*/);
        return true;
    }

    if (isBackward)
    {
        float throttle = isTurning
                             ? 0.0f     // no move
                             : EPSILON; // min throttle to activate reverse
        this->response = ControllerOutput(false /*speed2*/, false /*speed3*/, throttle, !isTurning /*reverse*/);
        return true;
    }

    // spinning
    float throttle = isTurning
                         ? EPSILON      // min throttle to activate reverse
                         : SPEED_1_MAX; // equalize reverse constant speed
    this->response = ControllerOutput(false, false, throttle, isTurning);
    return true;
}

ControllerOutput Controller::getDrive() { return this->response; }

Controller leftController = Controller(true);
Controller rightController = Controller(false);