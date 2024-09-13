#include <DAC.h>
#include <Utils.h>

void Dac::begin()
{
    this->i2c.write(this->address, 0x00);
}

void Dac::write(float throttle)
{
    auto signalValue = mapNumber(throttle, 0.0f, 1.0f, THROTTLE_DESIRE_MIN, THROTTLE_DESIRE_MAX);
    this->write(signalValue);
}

void Dac::write(int value12Bit)
{
    this->i2c.write(this->address, value12Bit);
}
