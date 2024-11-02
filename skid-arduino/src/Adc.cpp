// #include <Adc.h>
// #include <Utils.h>
// #include <Pins.h>

// void Adc::begin() const
// {
//     for (auto i = 0; i < this->count; i++)
//         pinMode(this->pins[i], INPUT);
// }

// bool read8Bit(const uint8_t slaveAddress, uint8_t &value) const;
// bool read12Bit(const uint8_t slaveAddress, uint12_t &value) const;
// bool read16Bit(const uint8_t slaveAddress, uint16_t &value) const;

// void Adc::read()
// {
//     for (auto i = 0; i < this->count; i++)
//         this->values[i] = analogRead(this->pins[i]);
// }

// uint10_t Adc::getInteger(size_t index) const
// {
//     assert(index >= 1);
//     assert(index <= this->count);
//     return this->values[index - 1];
// }

// ratio_t Adc::getRatio(const size_t index, const uint10_t min, const uint10_t max) const
// {
//     auto value = this->getInteger(index);
//     return mapNumber(value, min, max, RATIO_MIN, RATIO_MAX);
// }
