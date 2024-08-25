#include <Arduino.h>
#include <Controller.h>
#include <Utils.h>

bool isChanged(ControllerInput last, ControllerInput next)
{
    return abs(next.desiredThrottle - next.currentThrottle) > EPSILON    // throttle is being accelerated smoothly up to desired value
           || abs(last.desiredThrottle - next.desiredThrottle) > EPSILON // no relevant desired throttle change
           || abs(last.steering - next.steering) > EPSILON;              // no relevant steering change
}

Controller::Controller(bool isLeft) { this->turnSign = isLeft ? -1 : 1; }

bool Controller::tryUpdate(ControllerInput request)
{
    // todo: remove once the current throttle feature is supported
    request.currentThrottle = request.currentThrottle == 0 ? this->response.throttle : request.currentThrottle;

    if (!isChanged(this->request, request))
        return false; // nothing to recalculate

    int timeDelta = max(0, millis() - this->lastTime); // the time since last calculation
    this->lastTime = millis();

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
        // [ ] proportional when throttle is directly mapped on speed scale [1..2]
        // [x] smoothly accelerating throttle up by increasing 20% to desired according to time
        // [ ] smoothly accelerating throttle up to requested value by increasing 20% current throttle to desired

        // smoothly accelerated throttle up to its desired value
        float acceleratedThrottle = min(currentThrottle + THROTTLE_ACC_INC(timeDelta), desiredThrottle);

        if (isTurning)
        {
            // impact accelerated throttle proportionally to its desired and steering values
            float turningFactor = (1.0f - steering * STEERING_MULT / desiredThrottle);
            // drop throttle down to turn
            acceleratedThrottle = max(acceleratedThrottle * turningFactor, THROTTLE_MIN);
        }

        this->response = ControllerOutput(acceleratedThrottle, false /*reverse*/);
        return true;
    }

    if (isBackward)
    {
        float throttle = isTurning
                             ? 0.0f          // no move
                             : THROTTLE_REV; // min throttle to activate reverse
        this->response = ControllerOutput(throttle, !isTurning /*reverse*/);
        return true;
    }

    // spinning
    this->response = ControllerOutput(THROTTLE_REV, isTurning /*reverse*/);
    return true;
}

ControllerOutput Controller::getDrive() { return this->response; }

Controller leftController = Controller(true);
Controller rightController = Controller(false);
