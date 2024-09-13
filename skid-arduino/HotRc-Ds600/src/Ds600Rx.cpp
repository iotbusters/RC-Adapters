#include <Ds600Rx.h>
#include <Utils.h>
#include <Pins.h>

float channel(byte channel, int min, int max)
{
    int channelValue = pulseIn(channel, HIGH);
    channelValue = compareExchange(channelValue, (min + max) / 2, 0);
    return mapNumber(channelValue, min, max, -1.0f, 1.0f);
}

void Ds600Rx::begin()
{
    // ensure non-zero as actual zero in the center
    // a pull-down 6-7k resistor on each pin is required

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
    return channel(CH1, SIG_STEERING_MIN, SIG_STEERING_MAX);
}

float Ds600Rx::channel2()
{
    return channel(CH2, SIG_THROTTLE_MIN, SIG_THROTTLE_MAX);
}