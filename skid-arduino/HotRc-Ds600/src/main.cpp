
// todo:
// - introduce auto/manual calibration
// - move relays to I2C
// - utilize auxiliary pins

#include <arduino.h>
#include <main.h>
#include <controller.h>
#include <tools.h>
#include <wire.h>

extern Controller leftController;
extern Controller rightController;

void setup()
{
  Serial.begin(9600); // debugging serial
  Wire.begin();       // I2C peripherals

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
  pinMode(THROTTLE_CUR_L1, INPUT);
  pinMode(THROTTLE_CUR_L2, INPUT);
  pinMode(THROTTLE_CUR_L3, INPUT);
  pinMode(THROTTLE_CUR_R1, INPUT);
  pinMode(THROTTLE_CUR_R2, INPUT);
  pinMode(THROTTLE_CUR_R3, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BREAKS, OUTPUT);
  pinMode(REVERSE_L, OUTPUT);
  pinMode(REVERSE_R, OUTPUT);
  pinMode(SPEED_L2, OUTPUT);
  pinMode(SPEED_L3, OUTPUT);
  // pinMode(SPEED_R2, OUTPUT);
  // pinMode(SPEED_R3, OUTPUT);
}

// float x;

void loop()
{
  int speedLeft1Signal = analogRead(THROTTLE_CUR_L1);
  int speedLeft2Signal = analogRead(THROTTLE_CUR_L2);
  int speedLeft3Signal = analogRead(THROTTLE_CUR_L3);
  int speedRight1Signal = analogRead(THROTTLE_CUR_R1);
  int speedRight2Signal = analogRead(THROTTLE_CUR_R2);
  int speedRight3Signal = analogRead(THROTTLE_CUR_R3);
  int steeringSignal = pulseIn(CH1, HIGH);
  int throttleSignal = pulseIn(CH2, HIGH);
  // int aux3Signal = pulseIn(CH3, HIGH);
  // int aux4Signal = pulseIn(CH4, HIGH);
  // int aux5Signal = pulseIn(CH5, HIGH);
  // int aux6Signal = pulseIn(CH6, HIGH);

  float steering = (float)(steeringSignal - STEERING_MIDDLE_VALUE) / (STEERING_MIDDLE_VALUE - STEERING_MIN_VALUE);
  float desiredThrottle = (float)(throttleSignal - THROTTLE_MIDDLE_VALUE) / (THROTTLE_MIDDLE_VALUE - THROTTLE_MIN_VALUE);

  float currentThrottleLeft = (float)avg({speedLeft1Signal, speedLeft2Signal, speedLeft3Signal}) / SPEED_MAX_VALUE;
  float currentThrottleRight = (float)avg({speedRight1Signal, speedRight2Signal, speedRight3Signal}) / SPEED_MAX_VALUE;

  InputData leftRequest = InputData(currentThrottleLeft, desiredThrottle, steering),
            rightRequest = InputData(currentThrottleRight, desiredThrottle, steering);
  OutputData leftResponse, rightResponse;

  bool isLeftChanged = leftController.tryCalculate(leftRequest, &leftResponse);
  bool isRightChanged = rightController.tryCalculate(rightRequest, &rightResponse);
  if (!isLeftChanged && !isRightChanged)
  {
    delay(200);
    return; // skip if no changes
  }

  digitalWrite(LED_BUILTIN, HIGH);

  int throttleLeftValue = fmap(leftResponse.throttle, 0.0f, 1.0f, (float)INT_12BIT_MIN_VALUE, (float)INT_12BIT_MAX_VALUE);
  int throttleRightValue = fmap(rightResponse.throttle, 0.0f, 1.0f, (float)INT_12BIT_MIN_VALUE, (float)INT_12BIT_MAX_VALUE);

  Serial.print("Left: (");
  Serial.print(1 + leftResponse.speed2 + leftResponse.speed3);
  Serial.print(!leftResponse.isReversed ? "F) " : "R) ");
  Serial.print(leftResponse.throttle);
  Serial.print("  Right: (");
  Serial.print(1 + rightResponse.speed2 + rightResponse.speed3);
  Serial.print(!rightResponse.isReversed ? "F) " : "R) ");
  Serial.println(rightResponse.throttle);

  digitalWrite(SPEED_L2, leftResponse.speed2);
  digitalWrite(SPEED_L3, leftResponse.speed3);
  // digitalWrite(SPEED_R2, rightResponse.speed2);
  // digitalWrite(SPEED_R3, rightResponse.speed3);

  digitalWrite(BREAKS, leftResponse.Breaks() && rightResponse.Breaks());
  digitalWrite(REVERSE_L, leftResponse.isReversed);
  digitalWrite(REVERSE_R, rightResponse.isReversed);

  Wire.beginTransmission(I2C_THROTTLE_LEFT);
  Wire.write(throttleLeftValue >> 8);  // high 8 bits
  Wire.write(throttleLeftValue & 255); // low 8 bits
  Wire.endTransmission();

  Wire.beginTransmission(I2C_THROTTLE_RIGHT);
  Wire.write(throttleRightValue >> 8);  // high 8 bits
  Wire.write(throttleRightValue & 255); // low 8 bits
  Wire.endTransmission();

  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}