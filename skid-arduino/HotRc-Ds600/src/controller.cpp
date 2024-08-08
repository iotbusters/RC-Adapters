#include <Arduino.h>
#include <Controller.h>
#include <Utils.h>

byte getSpeedLevel(float throttle)
{
    for (byte speed = 1; speed < size(maxSpeeds); speed++)
        if (throttle <= maxSpeeds[speed])
            return speed;

    return 1; // unknown scenario
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

        // smoothly accelerated throttle up to its desired value
        float acceleratedThrottle = min(currentThrottle * SPEED_ACC_MULT, desiredThrottle);

        if (isTurning)
        {
            // impact accelerated throttle proportionally to its desired and steering values
            float turningFactor = (1.0f - steering * STEERING_MULT / desiredThrottle);
            // drop throttle down to turn
            acceleratedThrottle = max(acceleratedThrottle * turningFactor, THROTTLE_MIN);
        }

        // find speed level by current throttle
        byte speed = getSpeedLevel(desiredThrottle);
        // map proportionally current throttle to selected speed throttle [1-100%]
        float speedScaledThrottle = mapNumber(acceleratedThrottle, maxSpeeds[speed - 1], maxSpeeds[speed], THROTTLE_MIN, 1.0f);
        this->response = ControllerOutput(speed == 3 /*isHighSpeed*/, speedScaledThrottle, false /*reverse*/);
        return true;
    }

    if (isBackward)
    {
        float throttle = isTurning
                             ? 0.0f  // no move
                             : 0.5f; // min throttle to activate reverse
        this->response = ControllerOutput(1 /*speed*/, throttle, !isTurning /*reverse*/);
        return true;
    }

    // spinning
    float throttle = isTurning
                         ? 0.5f  // min throttle to activate reverse
                         : 0.5f; // equalize reverse constant speed
    this->response = ControllerOutput(1 /*speed*/, throttle, isTurning /*reverse*/);
    return true;
}

ControllerOutput Controller::getDrive() { return this->response; }

Controller leftController = Controller(true);
Controller rightController = Controller(false);