#include <Arduino.h>
#include <I2C.h>
#include <Relays.h>

void Relays::begin()
{
    this->i2c.write(I2C_ADR_RELAYS, 0xFF);
}

void Relays::write(bool breaks, RelayWheelInput left, RelayWheelInput right)
{
    byte relays = 0x00;
    relays |= (byte)!breaks << I2C_RELAY_BREAKS;
    relays |= (byte)left.reversed << I2C_RELAY_REVERSE_L;
    relays |= (byte)right.reversed << I2C_RELAY_REVERSE_R;
    relays |= (byte)left.lowSpeed << I2C_RELAY_SPEED2_L;
    relays |= (byte)right.lowSpeed << I2C_RELAY_SPEED2_R;
    this->i2c.write(I2C_ADR_RELAYS, ~relays); // inverted bits
}
