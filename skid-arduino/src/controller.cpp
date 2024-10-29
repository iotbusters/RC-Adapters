#include <Arduino.h>
#include <Controller.h>
#include <Utils.h>

bool Controller::isChanged(const ControllerInput &next)
{
    return abs(this->output.throttle - next.desiredThrottle) > EPSILON          // throttle is being accelerated smoothly up to desired value
           || abs(this->input.desiredThrottle - next.desiredThrottle) > EPSILON // no relevant desired throttle change
           || abs(this->input.steering - next.steering) > EPSILON;              // no relevant steering change
}

bool Controller::tryUpdate(const ControllerInput &input)
{
    if (!this->isChanged(input))
        return false; // nothing to recalculate

    auto timeDelta = millis() - this->lastTime; // the time since last calculation
    this->lastTime = millis();

    this->input = input;

    auto steering = abs(input.steering);
    auto desiredThrottle = abs(input.desiredThrottle);

    if (steering < EPSILON && desiredThrottle < EPSILON)
    {
        this->input = ControllerInput::idle;
        this->output = ControllerOutput::idle;
        return true; // got back to idle
    }

    auto isForward = input.desiredThrottle > EPSILON;
    auto isBackward = input.desiredThrottle < -EPSILON;
    auto isTurning = input.steering * this->turnSign > EPSILON;

    if (isForward)
    {
        // throttle strategies
        // [ ] proportional when throttle is directly mapped on speed scale [1..2]
        // [x] smoothly accelerating throttle up by increasing 20% to desired according to time
        // [ ] smoothly accelerating throttle up to requested value by increasing 20% current throttle to desired

        // smoothly accelerated throttle up to its desired value
        auto acceleratedThrottle = min(this->output.throttle + THROTTLE_ACC_INC(timeDelta), desiredThrottle);

        if (isTurning)
        {
            // impact accelerated throttle proportionally to its desired and steering values
            auto turningFactor = (1.0f - steering * STEERING_MULT / desiredThrottle);
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

const ControllerOutput &Controller::getOutput() const { return this->output; }
