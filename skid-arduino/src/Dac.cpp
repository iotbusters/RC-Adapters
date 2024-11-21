#include <DAC.h>
#include <Utils.h>

void Dac::begin() const {
    this->write(uint12_t(0x00)); // reset value.
}

/// @brief writes ratio as 12bit integer.
/// @param ratio proportion between min and max of 12bit unsigned integer.
void Dac::write(const ufract_t value) const {
    auto fract = mapNumber(value, UFRACT_MIN, UFRACT_MAX, UINT12_MIN, UINT12_MAX);
    this->write(fract);
}

void Dac::write(const uint12_t value) const { this->i2c.write(this->address, value); }
