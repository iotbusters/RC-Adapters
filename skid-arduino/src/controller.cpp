#include <Arduino.h>
#include <Controller.h>
#include <Utils.h>
#include <memory.h>

bool Controller::isChanged(const ControllerInput &input) {
    // return accelerating || abs(this->input.desiredThrottle - input.desiredThrottle) > EPSILON // no relevant desired throttle change
    //        || abs(this->input.steering - input.steering) > EPSILON;                           // no relevant steering change
    return accelerating || (this->input.desiredThrottle - input.desiredThrottle).absolute() > EPSILON // no relevant desired throttle change
           || (this->input.steering - input.steering).absolute() > EPSILON;                           // no relevant steering change
}

bool Controller::tryUpdate(const ControllerInput &input) {
    if (!this->isChanged(input))
        return false; // nothing to recalculate

    // println("updating1");
    //  delay(50);
    // printMemory("updating1");
    // println("changed: previous(%d), next(%d)", int(100 * (float)this->input.desiredThrottle), (int)(100 * (float)input.desiredThrottle));
    this->input = input;
    // Serial.println("updating1.1");
    // delay(50);
    auto timeDelta = millis() - this->lastTime; // the time since last calculation
    // Serial.println("updating1.2");
    // delay(50);
    this->lastTime = millis();

    // Serial.println("updating2");
    // delay(50);
    // auto steering = abs(input.steering);
    // auto desiredThrottle = abs(input.desiredThrottle);
    auto steering = input.steering.absolute();
    auto desiredThrottle = input.desiredThrottle.absolute();

    if (steering < EPSILON && desiredThrottle < EPSILON) {
        // Serial.println("changed: close to zero");
        this->output = ControllerOutput::idle;
        return true; // got back to idle
    }
    // Serial.println("updating3");
    auto isForward = input.desiredThrottle > EPSILON;
    auto isBackward = input.desiredThrottle < -EPSILON;
    // auto isTurning = input.steering * this->turnSign > EPSILON;
    auto isTurning = input.steering.sign(this->turnSign) > EPSILON;

    if (isForward) {
        // throttle strategies
        // [ ] proportional when throttle is directly mapped on speed scale [1..2]
        // [x] smoothly accelerating throttle up by increasing 20% to desired according to time
        // [ ] smoothly accelerating throttle up to requested value by increasing 20% current throttle to desired

        // smoothly accelerated throttle up to its desired value
        auto acceleratedThrottle = min(this->output.desiredThrottle + THROTTLE_ACC_INC(timeDelta), desiredThrottle);

        if (isTurning) {
            // impact accelerated throttle proportionally to its desired and steering values
            auto turningFactor = (UFRACT_MAX - steering * STEERING_MULT / desiredThrottle);
            // drop throttle down to turn
            acceleratedThrottle = max(acceleratedThrottle * turningFactor, UFRACT_MIN);
        }

        // Serial.println("updating: forward");
        this->accelerating = acceleratedThrottle < desiredThrottle;
        this->output = ControllerOutput(acceleratedThrottle, false /*reverse*/);
        return true;
    }

    if (isBackward) {
        // Serial.println("updating: backward");
        auto throttle = isTurning ? UFRACT_MIN    // no move
                                  : THROTTLE_REV; // min throttle to activate reverse
        this->accelerating = false;
        this->output = ControllerOutput(throttle, !isTurning /*reverse*/);
        return true;
    }

    // printMemory("updating: spinning");
    // spinning
    this->output = ControllerOutput(THROTTLE_REV, isTurning /*reverse*/);
    return true;
}

const ControllerOutput &Controller::getOutput() const { return this->output; }
