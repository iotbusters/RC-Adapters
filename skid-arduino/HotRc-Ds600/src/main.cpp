// todo:
// - a controller per a drive (6x6)
// - introduce auto/manual calibration
// - utilize auxiliary pins

#include <Arduino.h>
#include <Controller.h>
#include <Main.h>
#include <Utils.h>
#include <Wire.h>

extern Controller leftController;
extern Controller rightController;

void setup()
{
  Serial.begin(9600); // debugging serial
  Wire.begin();       // connecting the master to I2C peripherals

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  // note: unused
  // pinMode(CH3, INPUT);
  // pinMode(CH4, INPUT);
  // pinMode(CH5, INPUT);
  // pinMode(CH6, INPUT);

  pinMode(THROTTLE_CUR_L1, INPUT_PULLUP);
  pinMode(THROTTLE_CUR_L2, INPUT);
  pinMode(THROTTLE_CUR_L3, INPUT);
  pinMode(THROTTLE_CUR_R1, INPUT);
  pinMode(THROTTLE_CUR_R2, INPUT);
  pinMode(THROTTLE_CUR_R3, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  int channel1Value = pulseIn(CH1, HIGH);
  int channel2Value = pulseIn(CH2, HIGH);
  // ensure non-zero as actual zero in the center
  // a pull-down 6-7k resistor on each pin is required
  channel1Value = compareExchange(channel1Value, STEERING_DEFAULT, 0);
  channel2Value = compareExchange(channel2Value, THROTTLE_DEFAULT, 0);
  // note: unused
  // int channel3Value = pulseIn(CH3, HIGH);
  // int channel4Value = pulseIn(CH4, HIGH);
  // int channel5Value = pulseIn(CH5, HIGH);
  // int channel6Value = pulseIn(CH6, HIGH);
  int leftCurrentThrottle1Value = analogRead(THROTTLE_CUR_L1);
  int leftCurrentThrottle2Value = analogRead(THROTTLE_CUR_L2);
  int leftCurrentThrottle3Value = analogRead(THROTTLE_CUR_L3);
  int rightCurrentThrottle1Value = analogRead(THROTTLE_CUR_R1);
  int rightCurrentThrottle2Value = analogRead(THROTTLE_CUR_R2);
  int rightCurrentThrottle3Value = analogRead(THROTTLE_CUR_R3);

  float steering = mapNumber(channel1Value, STEERING_MIN, STEERING_MAX, -1.0f, 1.0f);
  float desiredThrottle = mapNumber(channel2Value, THROTTLE_MIN, THROTTLE_MAX, -1.0f, 1.0f);

  float leftCurrentThrottle1 = mapNumber(leftCurrentThrottle1Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  float leftCurrentThrottle2 = mapNumber(leftCurrentThrottle2Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  float leftCurrentThrottle3 = mapNumber(leftCurrentThrottle3Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  float rightCurrentThrottle1 = mapNumber(rightCurrentThrottle1Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  float rightCurrentThrottle2 = mapNumber(rightCurrentThrottle2Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  float rightCurrentThrottle3 = mapNumber(rightCurrentThrottle3Value, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
  // ensure non-zero to cover a lost wire connection case
  // a pull-down 6-7k resistor on each pin is required
  // leftCurrentThrottle1 = compareExchange(leftCurrentThrottle1, desiredThrottle, 0.0f);
  // leftCurrentThrottle2 = compareExchange(leftCurrentThrottle2, desiredThrottle, 0.0f);
  // leftCurrentThrottle3 = compareExchange(leftCurrentThrottle3, desiredThrottle, 0.0f);
  // rightCurrentThrottle1 = compareExchange(rightCurrentThrottle1, desiredThrottle, 0.0f);
  // rightCurrentThrottle2 = compareExchange(rightCurrentThrottle2, desiredThrottle, 0.0f);
  // rightCurrentThrottle3 = compareExchange(rightCurrentThrottle3, desiredThrottle, 0.0f);

  float leftCurrentThrottle = minArray({leftCurrentThrottle1, leftCurrentThrottle2, leftCurrentThrottle3});
  float rightCurrentThrottle = minArray({rightCurrentThrottle1, rightCurrentThrottle2, rightCurrentThrottle3});

  ControllerInput leftRequest = ControllerInput(leftCurrentThrottle, desiredThrottle, steering),
                  rightRequest = ControllerInput(rightCurrentThrottle, desiredThrottle, steering);

  bool isLeftChanged = leftController.tryUpdate(leftRequest);
  bool isRightChanged = rightController.tryUpdate(rightRequest);
  if (!isLeftChanged && !isRightChanged)
  {
    delay(300);
    return; // skip if no changes
  }

  digitalWrite(LED_BUILTIN, HIGH);

  ControllerOutput leftDrive = leftController.getDrive();
  ControllerOutput rightDrive = rightController.getDrive();

  // ----- debug information -----
  Serial.print("Left: (");
  Serial.print(1 + leftDrive.speed2 + leftDrive.speed3);
  Serial.print(!leftDrive.isReversed ? "F) " : "R) ");
  if (!leftDrive.Breaks())
    Serial.print(leftDrive.throttle, 3);
  else
    Serial.print("[break]");
  Serial.print("  Right: (");
  Serial.print(1 + rightDrive.speed2 + rightDrive.speed3);
  Serial.print(!rightDrive.isReversed ? "F) " : "R) ");
  if (!rightDrive.Breaks())
    Serial.println(rightDrive.throttle, 3);
  else
    Serial.println("[break]");
  // ----- debug information -----

  int throttleLeftValue = mapNumber(leftDrive.throttle, 0.0f, 1.0f, INT_12BIT_MIN, INT_12BIT_MAX);
  Wire.beginTransmission(I2C_ADR_THROTTLE_L);
  Wire.write(throttleLeftValue >> 8);   // high 8 bits
  Wire.write(throttleLeftValue & 0xFF); // low 8 bits
  Wire.endTransmission();

  int throttleRightValue = mapNumber(rightDrive.throttle, 0.0f, 1.0f, INT_12BIT_MIN, INT_12BIT_MAX);
  Wire.beginTransmission(I2C_ADR_THROTTLE_R);
  Wire.write(throttleRightValue >> 8);   // high 8 bits
  Wire.write(throttleRightValue & 0xFF); // low 8 bits
  Wire.endTransmission();

  byte relays = 0x00;
  bool breaks = leftDrive.Breaks() || rightDrive.Breaks();
  relays |= (byte)breaks << I2C_RELAY_BREAKS;
  relays |= (byte)leftDrive.isReversed << I2C_RELAY_REVERSE_L;
  relays |= (byte)rightDrive.isReversed << I2C_RELAY_REVERSE_R;
  relays |= (byte)leftDrive.speed2 << I2C_RELAY_SPEED_L2;
  relays |= (byte)leftDrive.speed3 << I2C_RELAY_SPEED_L3;
  relays |= (byte)rightDrive.speed2 << I2C_RELAY_SPEED_R2;
  relays |= (byte)rightDrive.speed3 << I2C_RELAY_SPEED_R3;
  Wire.beginTransmission(I2C_ADR_RELAYS);
  Wire.write(relays);
  Wire.endTransmission();

  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
}