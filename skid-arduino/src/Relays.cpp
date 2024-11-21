#include <Arduino.h>
#include <I2C.h>
#include <Relays.h>
#include <memory.h>

void Relays::begin() const { this->i2cMaster.write(I2C_ADR_RELAYS, 0xFF); }

void Relays::write(const bool breaks, const RelayWheelInput &left, const RelayWheelInput &right) const {
    uint8_t relays = 0x00;
    relays |= (uint8_t)left.highSpeed << I2C_RELAY_SPEED2_L;
    relays |= (uint8_t)left.reversed << I2C_RELAY_REVERSE_L;
    relays |= (uint8_t)!breaks << I2C_RELAY_BREAKS;
    relays |= (uint8_t)right.reversed << I2C_RELAY_REVERSE_R;
    relays |= (uint8_t)right.highSpeed << I2C_RELAY_SPEED2_R;
    this->i2cMaster.write(I2C_ADR_RELAYS, ~relays); // inverted bits
}
