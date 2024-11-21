// #include <StateMachine.h>

// void logOutputs(const ControllerOutput &left, const ControllerOutput &right);

// bool StateMachine::tryRead()
// {
//     return this->primaryRx.tryRead() || this->secondaryRx.tryRead();
// }

// void StateMachine::begin()
// {
//     Serial.begin(9600);
//     this->primaryRx.begin();
//     this->secondaryRx.begin();
//     this->i2cMaster.begin();
//     // this->leftThrottle.begin();
//     // this->rightThrottle.begin();
//     this->relays.begin();

//     pinMode(LED_BUILTIN, OUTPUT);
// }

// void StateMachine::next()
// {
//     if (!tryRead())
//         return; // no RC links.

//     if (this->operationRunTime > millis())
//         return; // delayed operation.

//     (this->*operation)();
// }

// void StateMachine::schedule(void (StateMachine::*nextOp)(), ulong delay)
// {
//     if (this->operation != nextOp)
//         this->operation = nextOp;
//     if (delay != 0)
//         this->operationRunTime = millis() + delay;
// }

// void StateMachine::updateControllers()
// {
//     const auto rxOutput = this->primaryRx.getOutput() || this->secondaryRx.getOutput();
//     const auto input = ControllerInput(rxOutput.steering, rxOutput.throttle);
//     const auto isLeftUpdated = this->leftController.tryUpdate(input);
//     const auto isRightUpdated = this->rightController.tryUpdate(input);
//     if (!isLeftUpdated && !isRightUpdated)
//     {
//         this->schedule(&StateMachine::updateControllers, 200);
//         return; // skip if no updates
//     }

//     const auto &leftOutput = this->leftController.getOutput(),
//                &rightOutput = this->rightController.getOutput();
//     logOutputs(leftOutput, rightOutput);

//     digitalWrite(LED_BUILTIN, HIGH);

//     this->schedule(&StateMachine::writeRelays);
// }

// void StateMachine::writeRelays()
// {
//     const auto &leftOutput = this->leftController.getOutput(),
//                &rightOutput = this->rightController.getOutput();
//     const auto breaks = leftOutput.breaks() || rightOutput.breaks();
//     const auto leftWheel = RelayWheelInput(leftOutput.reversed, leftOutput.highSpeed);
//     const auto rightWheel = RelayWheelInput(rightOutput.reversed, rightOutput.highSpeed);
//     this->relays.write(breaks, leftWheel, rightWheel);

//     this->schedule(&StateMachine::writeThrottles, 100);
// }

// void StateMachine::writeThrottles()
// {
//     const auto &leftOutput = this->leftController.getOutput(),
//                &rightOutput = this->rightController.getOutput();
//     // this->leftThrottle.write(leftOutput.speedThrottle);
//     // this->rightThrottle.write(rightOutput.speedThrottle);

//     digitalWrite(LED_BUILTIN, LOW);

//     this->schedule(&StateMachine::updateControllers, 300);
// }

// void log(const char label[], const ControllerOutput &output)
// {
//     Serial.print(label);
//     Serial.print(": (");
//     Serial.print(output.highSpeed ? 2 : 1);
//     Serial.print(!output.reversed ? "F) " : "R) ");
//     if (!output.breaks())
//     {
//         Serial.print(output.speedThrottle, 3);
//         Serial.print("(");
//         Serial.print(output.throttle, 3);
//         Serial.print(")");
//     }
//     else
//         Serial.print("[break]");
// }

// void logOutputs(const ControllerOutput &left, const ControllerOutput &right)
// {
//     log("Left", left);
//     log("  Right", right);
//     Serial.println();
// }
