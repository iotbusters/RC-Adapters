#pragma once

#include <Arduino.h>
#include <Controller.h>
#include <Debug.h>
#include <Relays.h>
#include <Dac.h>
#include <Adc.h>
#include <Ds600Rx.h>
#include <Es900Rx.h>
#include <Utils.h>
#include <Pins.h>

class StateMachine
{
private:
    void (StateMachine::*operation)() = &StateMachine::updateControllers;
    unsigned long operationRunTime = millis();

    Controller leftController = Controller(true);
    Controller rightController = Controller(false);
    Es900Rx primaryRx;
    Ds600Rx secondaryRx;

    const I2c i2c = I2c(Wire);
    const Dac leftThrottle = Dac(this->i2c, I2C_ADR_THROTTLE_L);
    const Dac rightThrottle = Dac(this->i2c, I2C_ADR_THROTTLE_R);
    const Relays relays = Relays(this->i2c);

    void schedule(void (StateMachine::*nextOp)(), unsigned long delay);
    void updateControllers();
    void writeRelays();
    void writeThrottles();

    bool tryRead();

public:
    void begin();
    void next();
};
