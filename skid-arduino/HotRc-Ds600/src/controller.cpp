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

bool Controller::tryUpdate(ControllerInput input)
{
    // todo: remove once the current throttle feature is supported
    input.currentThrottle = input.currentThrottle == 0
                                ? this->output.throttle
                                : input.currentThrottle;

    if (!isChanged(this->input, input))
        return false; // nothing to recalculate

    int timeDelta = max(0, millis() - this->lastTime); // the time since last calculation
    this->lastTime = millis();

    this->input = input;

    float steering = abs(input.steering);
    float currentThrottle = abs(input.currentThrottle);
    float desiredThrottle = abs(input.desiredThrottle);

    if (steering < EPSILON && desiredThrottle < EPSILON)
    {
        this->input = ControllerInput::idle;
        this->output = ControllerOutput::idle;
        return true; // got back to idle
    }

    bool isForward = input.desiredThrottle > EPSILON;
    bool isBackward = input.desiredThrottle < -EPSILON;
    bool isTurning = input.steering * this->turnSign > EPSILON;

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

        this->output = ControllerOutput(acceleratedThrottle, false /*reverse*/);
        return true;
    }

    if (isBackward)
    {
        float throttle = isTurning
                             ? 0.0f          // no move
                             : THROTTLE_REV; // min throttle to activate reverse
        this->output = ControllerOutput(throttle, !isTurning /*reverse*/);
        return true;
    }

    // spinning
    this->output = ControllerOutput(THROTTLE_REV, isTurning /*reverse*/);
    return true;
}

ControllerOutput Controller::getOutput() { return this->output; }
