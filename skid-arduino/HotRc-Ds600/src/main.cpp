// todo:
// - introduce auto/manual calibration
// - utilize auxiliary pins

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

Controller leftController(true);
Controller rightController(false);

Es900Rx primaryRx;
Ds600Rx secondaryRx;

const I2c i2c(Wire);
const Dac leftThrottle(i2c, I2C_ADR_THROTTLE_L);
const Dac rightThrottle(i2c, I2C_ADR_THROTTLE_R);
const Relays relays(i2c);

auto nextUpdate = millis();

void setup()
{
  Debug::begin();
  primaryRx.begin();
  secondaryRx.begin();
  Wire.begin(); // connecting the master to I2C peripherals.

  leftThrottle.begin();
  rightThrottle.begin();
  relays.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  auto isLinked = primaryRx.tryLink();
  auto currentTime = millis();

  if (nextUpdate > currentTime)
    return;

  auto steering = 0.0;
  auto desiredThrottle = 0.0;

  if (isLinked)
  {
    steering = primaryRx.channel1();
    desiredThrottle = primaryRx.channel2();
  }
  else
  {
    steering = secondaryRx.channel1();
    desiredThrottle = secondaryRx.channel2();
  }

  const auto input = ControllerInput(desiredThrottle, steering);

  const auto isLeftChanged = leftController.tryUpdate(input);
  const auto isRightChanged = rightController.tryUpdate(input);

  if (!isLeftChanged && !isRightChanged)
  {
    nextUpdate = currentTime + 200;
    return; // skip if no changes
  }

  digitalWrite(LED_BUILTIN, HIGH);

  const auto &leftOutput = leftController.getOutput(),
             &rightOutput = rightController.getOutput();

  Debug::logLn(leftOutput, rightOutput);

  const auto breaks = leftOutput.breaks() || rightOutput.breaks();
  const auto leftWheel = RelayWheelInput(leftOutput.reversed, leftOutput.speed == 2);
  const auto rightWheel = RelayWheelInput(rightOutput.reversed, rightOutput.speed == 2);
  relays.write(breaks, leftWheel, rightWheel);

  delay(10); // ensure controller is updated to interpret throttle correctly

  leftThrottle.write(leftOutput.speedThrottle);
  rightThrottle.write(rightOutput.speedThrottle);

  digitalWrite(LED_BUILTIN, LOW);

  nextUpdate = currentTime + 200;
}
