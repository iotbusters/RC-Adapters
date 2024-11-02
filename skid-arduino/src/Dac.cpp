// #include <DAC.h>
// #include <Utils.h>

// void Dac::begin() const
// {
//     this->i2c.writeUint12(this->address, 0x00); // reset value.
// }

// void Dac::write(const ratio_t value, const uint12_t min, const uint12_t max) const
// {
//     auto value = mapNumber(value, RATIO_MIN, RATIO_MAX, min, max);
//     this->write(value);
// }

// void Dac::write(const uint12_t value) const
// {
//     this->i2c.writeUint12(this->address, value);
// }
