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
#include <Arduino-CRSF-software.h>
#include <Utils.h>
#include <HardwareSerial.h>
#include <Pins.h>

Controller leftController(true);
Controller rightController(false);
Adc leftCurrentThrottle(leftCurrentThrottlePins, size(leftCurrentThrottlePins));
Adc rightCurrentThrottle(rightCurrentThrottlePins, size(leftCurrentThrottlePins));
I2c i2c(Wire);
SoftwareSerial serial(UART_CRSF_RX, UART_CRSF_TX);
CRSF crossfire;
Dac leftThrottle(i2c, I2C_ADR_THROTTLE_L);
Dac rightThrottle(i2c, I2C_ADR_THROTTLE_R);
Relays relays(i2c);

float steering;
float desiredThrottle;
void onCrossfireDataReceived(const uint16_t channels[])
{
  steering = channels[0];
  desiredThrottle = channels[1];
}

void setup()
{
  // Debug and Arduino-CRSF use the same hardware serial,
  // so it was updated privately to Arduino-CRSF-software.
  Debug::begin();

  Ds600Rx::begin();

  Wire.begin(); // connecting the master to I2C peripherals

  // Debug and Arduino-CRSF use the same hardware serial,
  // so it was updated privately to Arduino-CRSF-software.
  serial.begin(115200);
  crossfire.begin(&serial, 115200);
  crossfire.onDataReceived(onCrossfireDataReceived);

  leftCurrentThrottle.begin();
  rightCurrentThrottle.begin();
  leftThrottle.begin();
  rightThrottle.begin();
  relays.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  auto currentLeftThrottle = leftCurrentThrottle.readCurrentThrottle();
  auto currentRightThrottle = rightCurrentThrottle.readCurrentThrottle();

  crossfire.readPacket();
  if (!crossfire.isConnected())
  {
    steering = Ds600Rx::channel1();
    desiredThrottle = Ds600Rx::channel2();
  }

  auto leftInput = ControllerInput(currentLeftThrottle, desiredThrottle, steering);
  auto rightInput = ControllerInput(currentRightThrottle, desiredThrottle, steering);

  auto isLeftChanged = leftController.tryUpdate(leftInput);
  auto isRightChanged = rightController.tryUpdate(rightInput);
  if (!isLeftChanged && !isRightChanged)
  {
    delay(300);
    return; // skip if no changes
  }

  digitalWrite(LED_BUILTIN, HIGH);

  ControllerOutput leftOutput = leftController.getOutput(),
                   rightOutput = rightController.getOutput();

  // Debug and Arduino-CRSF use the same hardware serial,
  // so it was updated privately to Arduino-CRSF-software.
  Debug::log(leftOutput, rightOutput);

  auto breaks = leftOutput.Breaks() || rightOutput.Breaks();
  auto leftWheel = RelayWheelInput(leftOutput.reversed, leftOutput.speed == 2);
  auto rightWheel = RelayWheelInput(rightOutput.reversed, rightOutput.speed == 2);
  relays.write(breaks, leftWheel, rightWheel);

  delay(100); // ensure controller is updated to interpret throttle correctly

  leftThrottle.write(leftOutput.speedThrottle);
  rightThrottle.write(rightOutput.speedThrottle);

  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}
