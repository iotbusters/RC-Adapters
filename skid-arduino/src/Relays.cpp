#include <Arduino.h>
#include <I2C.h>
#include <Relays.h>

void Relays::begin() const
{
    this->i2c.writeInteger12Bit(I2C_ADR_RELAYS, 0xFF);
}

void Relays::write(const bool &breaks, const RelayWheelInput &left, const RelayWheelInput &right) const
{
    byte relays = 0x00;
    relays |= (byte)!breaks << I2C_RELAY_BREAKS;
    relays |= (byte)left.reversed << I2C_RELAY_REVERSE_L;
    relays |= (byte)right.reversed << I2C_RELAY_REVERSE_R;
    relays |= (byte)left.highSpeed << I2C_RELAY_SPEED2_L;
    relays |= (byte)right.highSpeed << I2C_RELAY_SPEED2_R;

    this->i2c.writeByte(I2C_ADR_RELAYS, ~relays); // inverted bits
}
