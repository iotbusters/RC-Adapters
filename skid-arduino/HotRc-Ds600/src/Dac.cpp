#include <DAC.h>
#include <Utils.h>

void Dac::begin() const
{
    this->i2c.writeInteger12Bit(this->address, 0x00);
}

void Dac::write(const float &throttle) const
{
    auto signalValue = mapNumber(throttle, 0.0f, 1.0f, THROTTLE_DESIRE_MIN, THROTTLE_DESIRE_MAX);
    this->write(signalValue);
}

void Dac::write(const int &value12Bit) const
{
    this->i2c.writeInteger12Bit(this->address, value12Bit);
}
