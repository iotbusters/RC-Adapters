#pragma once

#include <Arduino.h>
#include <Controller.h>
#include <Relays.h>
#include <Dac.h>
#include <Ds600Rx.h>
#include <Es900Rx.h>
#include <Utils.h>
#include <Pins.h>

#define CH1 PIN_D2     // RC channel 1 (steering) pin
#define CH2 PIN_D3     // RC channel 2 (throttle) pin
#define CH3 PIN_D4     // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5     // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6     // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7     // RC channel 6 (auxiliary) pin
#define CRSF_RX PIN_D8 // UART RX pin to TX pin on crossfire receiver
#define CRSF_TX PIN_D9 // UART TX pin to RX pin on crossfire receiver

class StateMachine
{
private:
    void (StateMachine::*operation)() = &StateMachine::updateControllers;
    ulong operationRunTime = millis();

    Controller leftController = Controller(true);
    Controller rightController = Controller(false);
    HardwareSerial &logger = Serial;
    SoftwareSerial crossfireSerial = SoftwareSerial(CRSF_RX, CRSF_TX);
    Es900Rx primaryRx = Es900Rx(logger, this->crossfireSerial);
    Ds600Rx secondaryRx = Ds600Rx(logger, CH1, CH2, CH3, CH4);

    const I2cMaster &i2c = i2c.master();
    // const Dac leftThrottle = Dac(this->i2c, I2C_ADR_THROTTLE_L);
    // const Dac rightThrottle = Dac(this->i2c, I2C_ADR_THROTTLE_R);
    const Relays relays = Relays(this->i2c);

    // void schedule(void (StateMachine::*nextOp)(), ulong delay);
    void updateControllers();
    void writeRelays();
    void writeThrottles();

    bool tryRead();

public:
    void begin();

    void next();
};
