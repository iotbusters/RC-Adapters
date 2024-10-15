#include <Ds600Rx.h>
#include <Utils.h>
#include <Pins.h>
#include <Debug.h>

static float map(int value, int min, int max)
{
    value = compareExchange(value, (min + max) / 2, 0);
    return mapNumber(value, min, max, -1.0f, 1.0f);
}

void Ds600Rx::begin()
{
    // ensure non-zero as actual zero in the center
    // a pull-down 6-7k(10k) resistor on each pin is required

    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    // note: unused
    // pinMode(CH3, INPUT);
    // pinMode(CH4, INPUT);
    // pinMode(CH5, INPUT);
    // pinMode(CH6, INPUT);
}

float Ds600Rx::channel1()
{
    // Serial.print("CH1=");
    // Serial.println(pulseIn(CH1, HIGH));
    return map(pulseIn(CH1, HIGH), DS600RX_STEERING_MIN, DS600RX_STEERING_MAX);
}

float Ds600Rx::channel2()
{
    // Serial.print("CH2=");
    // Serial.println(pulseIn(CH2, HIGH));
    return map(pulseIn(CH2, HIGH), DS600RX_THROTTLE_MIN, DS600RX_THROTTLE_MAX);
}