#pragma once

#include <Arduino.h>

#define INT_10BIT_MIN 0
#define INT_10BIT_MAX 1023
// divider 24v to 5v: 27kOhms to PIN and 10kOhms to GND (test is needed)
// pull-down resistor: 6.8-10kOhms to GND (test is needed)
#define THROTTLE_CURRENT_MIN 190           // todo: measure min value when divider and pull-down resistor are applied
#define THROTTLE_CURRENT_MAX INT_10BIT_MAX // todo: measure max value when divider and pull-down resistor are applied

#define THROTTLE_DESIRE_MIN 862  // 0.85v      / 4.04v(5v arduino) * INT_12BIT_MAX
#define THROTTLE_DESIRE_MAX 4000 // 3.27v +20% / 4.04v(5v arduino) * INT_12BIT_MAX

class Adc
{
private:
    const byte *addresses;
    const byte size;

public:
    Adc(const byte *addresses, const byte size) : addresses(addresses), size(size) {}
    void begin() const;

    float readCurrentThrottle() const;
    int read12BitSignal() const;
};
