#pragma once

// #include <Arduino.h>
#include <Controller.h>
#include <Dac.h>
#include <Ds600Rx.h>
#include <Es900Rx.h>
#include <Pins.h>
#include <Relays.h>
#include <StateMachine/StateMachineBuilder.h>
#include <Utils.h>
#include <memory.h>

#define CH1 PIN_D2     // RC channel 1 (steering) pin
#define CH2 PIN_D3     // RC channel 2 (throttle) pin
#define CH3 PIN_D4     // RC channel 3 (auxiliary) pin
#define CH4 PIN_D5     // RC channel 4 (auxiliary) pin
#define CH5 PIN_D6     // RC channel 5 (auxiliary) pin
#define CH6 PIN_D7     // RC channel 6 (auxiliary) pin
#define CRSF_RX PIN_D8 // UART RX pin to TX pin on crossfire receiver
#define CRSF_TX PIN_D9 // UART TX pin to RX pin on crossfire receiver

void logOutputs(const ControllerOutput &left, const ControllerOutput &right);

Controller leftController(true);
Controller rightController(false);
SoftwareSerial crossfireSerial(CRSF_RX, CRSF_TX);
Es900Rx primaryRx(crossfireSerial);
Ds600Rx secondaryRx(CH1, CH2, CH3, CH4);

const I2cMaster i2cMaster = I2c(Wire).master();
const Relays relays = Relays(i2cMaster);
// const Dac leftThrottle = Dac(i2cMaster, I2C_ADR_THROTTLE_L);
// const Dac rightThrottle = Dac(i2cMaster, I2C_ADR_THROTTLE_R);

void *updateRxState(void *_) {
    const auto &leftOutput = leftController.getOutput();
    const auto &rightOutput = rightController.getOutput();
    logOutputs(leftOutput, rightOutput);
    digitalWrite(LED_BUILTIN, HIGH);
    return (void *)nullptr;
}

bool updateReadiness(void *_) {
    static ulong stateRunTime;
    if (stateRunTime == 0)
        stateRunTime = millis() + 200; // schedule delay 200ms
    if (stateRunTime > millis())
        return false; // delay transition
    stateRunTime = 0; // reset scheduler counter till next iteration

    const auto rxOutput = primaryRx.getOutput() || secondaryRx.getOutput();
    auto input = ControllerInput(rxOutput.steering, rxOutput.throttle);
    const auto isLeftUpdated = leftController.tryUpdate(input);
    const auto isRightUpdated = rightController.tryUpdate(input);

    println("update-rx?");
    return isLeftUpdated || isRightUpdated;
}

void *writeRelayState(void *_) {
    const auto &leftOutput = leftController.getOutput();
    const auto &rightOutput = rightController.getOutput();
    const auto breaks = leftOutput.breaks() || rightOutput.breaks();
    const auto leftWheel = RelayWheelInput(leftOutput.reversed, leftOutput.highSpeed);
    const auto rightWheel = RelayWheelInput(rightOutput.reversed, rightOutput.highSpeed);
    relays.write(breaks, leftWheel, rightWheel);

    return (void *)nullptr;
}

bool writeRelayReadiness(void *_) {
    static ulong stateRunTime;
    if (stateRunTime == 0)
        stateRunTime = millis() + 100; // schedule delay 100ms
    if (stateRunTime > millis())
        return false; // delay transition
    stateRunTime = 0; // reset scheduler counter till next iteration

    println("relay?");
    return true;
}

void *writeThrottleState(void *_) {
    const auto &leftOutput = leftController.getOutput();
    const auto &rightOutput = rightController.getOutput();
    // leftThrottle.write(leftOutput.speedThrottle);
    // rightThrottle.write(rightOutput.speedThrottle);
    digitalWrite(LED_BUILTIN, LOW);
    return (void *)nullptr;
}

bool writeThrottleReadiness(void *_) { return true; };

bool tryRead() { return primaryRx.tryRead() || secondaryRx.tryRead(); }

void begin() {
    Serial.begin(9600);
    printMemory();
    println("==== SETUP STARTED ====");

    primaryRx.begin();
    secondaryRx.begin();
    i2cMaster.begin();
    // leftThrottle.begin();
    // rightThrottle.begin();
    relays.begin();

    pinMode(LED_BUILTIN, OUTPUT);

    println("==== SETUP FINISHED ====");
    printMemory();
}

void log(const char label[], const ControllerOutput &output) {
    Serial.print(label);
    Serial.print(": (");
    Serial.print(output.highSpeed ? 2 : 1);
    Serial.print(!output.reversed ? "F) " : "R) ");
    if (!output.breaks()) {
        Serial.print((float)output.speedThrottle, 3);
        Serial.print("(");
        Serial.print((float)output.desiredThrottle, 3);
        Serial.print(")");
    } else
        Serial.print("[break]");
}

void logOutputs(const ControllerOutput &left, const ControllerOutput &right) {
    log("Left", left);
    log("  Right", right);
    Serial.println();
}
