#include <main.h>
#include <wire.h>

// I2C(SDA) PIN_A4
// I2C(SCL) PIN_A5
#define CH1 PIN_D2
#define CH2 PIN_D3
#define CH3 PIN_D4
#define CH4 PIN_D5
#define CH5 PIN_D6
#define CH6 PIN_D7
#define BREAK PIN_D8
// D9
#define REVERSE_LEFT PIN_D10
#define REVERSE_RIGHT PIN_D11
#define SPEED_2 PIN_D12
#define SPEED_3 PIN_D13
#define SPEED_LEFT_1_LEVEL PIN_D14
#define SPEED_LEFT_2_LEVEL PIN_D15
#define SPEED_LEFT_3_LEVEL PIN_D16
#define SPEED_RIGHT_1_LEVEL PIN_D17
#define SPEED_RIGHT_2_LEVEL PIN_D18
#define SPEED_RIGHT_3_LEVEL PIN_D19

#define INT_12BIT_MIN_VALUE 0
#define INT_12BIT_MAX_VALUE 4095

#define SPEED_MAX_VALUE 1023

#define STEERING_MIN_VALUE 560
#define STEERING_MAX_VALUE 2560
#define STEERING_MIDDLE_VALUE (STEERING_MAX_VALUE + STEERING_MIN_VALUE) / 2 // the point between left and right

#define THROTTLE_MIN_VALUE 1100
#define THROTTLE_MAX_VALUE 2100
#define THROTTLE_MIDDLE_VALUE (THROTTLE_MAX_VALUE + THROTTLE_MIN_VALUE) / 2 // the point between forward and reverse

#define SPEED_1_MAX 0.2 // 20%
#define SPEED_2_MAX 0.5 // 50%
#define SPEED_3_MAX 1.0 // 100%

#define EPSILON 0.02            // 2% or less is considered as a measurement error
#define STEERING_MULTIPLIER 0.4 // 40% of steering value is reduced from turning side throttle

float fmap(float val, float min, float max, float mappedMin, float mappedMax);
int avg(int *array);

float lastSpeed;
float lastSteering;
float lastThrottle;
float lastAux3;
float lastAux4;
float lastAux5;
float lastAux6;

void setup()
{
  Serial.begin(9600); // debugging serial

  Wire.begin();

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
  pinMode(SPEED_LEFT_1_LEVEL, INPUT);
  pinMode(SPEED_LEFT_2_LEVEL, INPUT);
  pinMode(SPEED_LEFT_3_LEVEL, INPUT);
  pinMode(SPEED_RIGHT_1_LEVEL, INPUT);
  pinMode(SPEED_RIGHT_2_LEVEL, INPUT);
  pinMode(SPEED_RIGHT_3_LEVEL, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BREAK, OUTPUT);
  pinMode(REVERSE_LEFT, OUTPUT);
  pinMode(REVERSE_RIGHT, OUTPUT);
  pinMode(SPEED_2, OUTPUT);
  pinMode(SPEED_3, OUTPUT);

  // 1. todo: set a speed level
  // requires 2 digit pins
  // throttle signal  0-20%: 1 speed (0-100% actual throttle)
  // throttle signal 20-50%: 2 speed (0-100% actual throttle)
  // throttle signal 50-100%: 3 speed (0-100% actual throttle)

  // 2. todo: speed measurement
  // - takes minimum value from all controllers
  // - considers throttle signal as desirable until actual speed is enough to raise
  // requires
  //  - 4-6 analog pins
  //  - responsibility breaking down to two arduino (left and right)

  // 3. todo: introduce [auto-] calibration
}

void loop()
{
  int speedLeft1Signal = analogRead(SPEED_LEFT_1_LEVEL);
  int speedLeft2Signal = analogRead(SPEED_LEFT_2_LEVEL);
  int speedLeft3Signal = analogRead(SPEED_LEFT_3_LEVEL);
  int speedRight1Signal = analogRead(SPEED_RIGHT_1_LEVEL);
  int speedRight2Signal = analogRead(SPEED_RIGHT_2_LEVEL);
  int speedRight3Signal = analogRead(SPEED_RIGHT_3_LEVEL);
  int steeringSignal = pulseIn(CH1, HIGH);
  int throttleSignal = pulseIn(CH2, HIGH);
  // int aux3Signal = pulseIn(CH3, HIGH);
  // int aux4Signal = pulseIn(CH4, HIGH);
  // int aux5Signal = pulseIn(CH5, HIGH);
  // int aux6Signal = pulseIn(CH6, HIGH);

  int speedSignals[] = {speedLeft1Signal, speedLeft2Signal, speedLeft3Signal, speedRight1Signal, speedRight2Signal, speedRight3Signal};
  float speed = avg(speedSignals) / SPEED_MAX_VALUE;

  float steering = (float)(steeringSignal - STEERING_MIDDLE_VALUE) / (STEERING_MIDDLE_VALUE - STEERING_MIN_VALUE);
  bool isTurningRight = steering > EPSILON;
  bool isTurningLeft = steering < -EPSILON;

  float throttle = (float)(throttleSignal - THROTTLE_MIDDLE_VALUE) / (THROTTLE_MIDDLE_VALUE - THROTTLE_MIN_VALUE);
  bool isDrivingForward = throttle > EPSILON;
  bool isDrivingBackward = throttle < -EPSILON;

  // skip if no relevant changes
  if (abs(lastSpeed - speed) < EPSILON && abs(lastThrottle - throttle) < EPSILON && abs(lastSteering - steering) < EPSILON)
  {
    delay(200);
    return;
  }

  // lastSpeed = speed;
  lastSteering = steering;
  lastThrottle = throttle;

  steering = abs(steering);
  throttle = abs(throttle);

  digitalWrite(LED_BUILTIN, HIGH);

  byte speedLevel;
  if (speed <= SPEED_1_MAX)
  {
    speedLevel = 1;
    float speedMultiplier = fmap(speed, 0, SPEED_1_MAX, 0, 1);
    // throttle = throttle * speedMultiplier;
  }
  else if (speed <= SPEED_2_MAX)
  {
    speedLevel = 2;
    float speedMultiplier = fmap(speed, SPEED_1_MAX, SPEED_2_MAX, 0, 1);
    // throttle = throttle * speedMultiplier;
  }
  else
  {
    speedLevel = 3;
    float speedMultiplier = fmap(speed, SPEED_2_MAX, SPEED_3_MAX, 0, 1);
    // throttle = throttle * speedMultiplier;
  }

  float throttleLeft;
  float throttleRight;
  bool canReverseLeft;
  bool canReverseRight;
  bool canBreak;

  if (isDrivingForward)
  {
    canReverseLeft = false;
    canReverseRight = false;
    canBreak = false;

    if (isTurningLeft)
    {
      throttleLeft = max(throttle - steering * STEERING_MULTIPLIER, EPSILON);
      throttleRight = throttle;
    }
    else if (isTurningRight)
    {
      throttleLeft = throttle;
      throttleRight = max(throttle - steering * STEERING_MULTIPLIER, EPSILON);
    }
    else
    {
      throttleLeft = throttle;
      throttleRight = throttle;
    }
  }
  else if (isDrivingBackward)
  {
    canBreak = false;

    if (isTurningLeft)
    {
      throttleLeft = EPSILON;
      throttleRight = throttle;
      canReverseLeft = false;
      canReverseRight = true;
    }
    else if (isTurningRight)
    {
      throttleLeft = throttle;
      throttleRight = EPSILON;
      canReverseLeft = true;
      canReverseRight = false;
    }
    else
    {
      throttleLeft = throttle;
      throttleRight = throttle;
      canReverseLeft = true;
      canReverseRight = true;
    }
  }
  else // reversal on the spot
  {
    if (isTurningLeft)
    {
      canBreak = false;
      throttleLeft = EPSILON;      // assuming it activates reverse
      throttleRight = SPEED_1_MAX; // assuming this is reverse speed
      canReverseLeft = true;
      canReverseRight = false;
    }
    else if (isTurningRight)
    {
      canBreak = false;
      throttleLeft = SPEED_1_MAX; // assuming this is reverse speed
      throttleRight = EPSILON;    // assuming it activates reverse
      canReverseLeft = false;
      canReverseRight = true;
    }
    else
    {
      canBreak = true;
      throttleLeft = 0;
      throttleRight = 0;
      canReverseLeft = false;
      canReverseRight = false;
    }
  }

  int throttleLeftValue = fmap(throttleLeft, 0, 1, INT_12BIT_MIN_VALUE, INT_12BIT_MAX_VALUE);
  int throttleRightValue = fmap(throttleRight, 0, 1, INT_12BIT_MIN_VALUE, INT_12BIT_MAX_VALUE);

  Serial.print("Left: ");
  Serial.print(canReverseLeft ? "(F) " : "(R) ");
  Serial.print(throttleLeft);
  Serial.print("  Right: ");
  Serial.print(canReverseRight ? "(F) " : "(R) ");
  Serial.println(throttleRight);

  if (speedLevel == 1)
  {
    digitalWrite(SPEED_2, LOW);
    digitalWrite(SPEED_3, LOW);
  }
  else if (speedLevel == 2)
  {
    digitalWrite(SPEED_2, HIGH);
    digitalWrite(SPEED_3, LOW);
  }
  else
  {
    digitalWrite(SPEED_2, HIGH);
    digitalWrite(SPEED_3, HIGH);
  }

  digitalWrite(BREAK, canBreak);
  digitalWrite(REVERSE_LEFT, canReverseLeft);
  digitalWrite(REVERSE_RIGHT, canReverseRight);

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

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int avg(int *array)
{
  int length = sizeof(array) / sizeof(array[0]);
  float sum = 0;

  for (int i = 0; i < length; i++)
    sum += array[i];

  return sum / length;
}