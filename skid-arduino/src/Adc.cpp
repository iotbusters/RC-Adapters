#include <Adc.h>
#include <Utils.h>
#include <Pins.h>

void Adc::begin() const
{
    for (auto i = 0; i < this->size; i++)
    {
        // todo: uncomment and move to dedicated module once the current throttle feature is supported
        // pinMode(this->addresses[i], INPUT);
    }
}

float Adc::readCurrentThrottle() const
{
    auto currentThrottleSignal = this->read12BitSignal();
    return mapNumber(currentThrottleSignal, THROTTLE_CURRENT_MIN, THROTTLE_CURRENT_MAX, 0.0f, 1.0f);
}

int Adc::read12BitSignal() const
{
    // note: ensure non-zero to cover a lost wire connection case by adding a pull-down 6-7k resistor on each pin
    // todo: ensure reducing controller's 24V current throttle signal to 5V by adding a voltage divider
    // consider the schema
    // | Controller     | ----[ R1/47kOhm ]----+----[ R2/10kOhm ]---- GND
    // | speed Vin(24V) |                      |
    //                                 | ADC (Arduino) |
    //                                 | Vout(4.8V)    |
    // due to formulas
    // Vout = Vin * (R2 / (R1 + R2))
    // Vin = Vout * ((R1 + R2) / R2)

    // todo: uncomment and move to dedicated module once the current throttle feature is supported
    // auto count = size(addresses);
    // int values[count];
    // for (byte i = 0; i < count; i++)
    //     values[i] = analogRead(addresses[i]);
    // return minArray(values);
    return 0;
}
