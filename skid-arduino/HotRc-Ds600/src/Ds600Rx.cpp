#include <Ds600Rx.h>
#include <Utils.h>
#include <Pins.h>
#include <Debug.h>

static uint16_t steeringLimits[2] = {DS600RX_STEERING_MIN, DS600RX_STEERING_MAX};
static uint16_t throttleLimits[2] = {DS600RX_THROTTLE_MIN, DS600RX_THROTTLE_MAX};

static void updateChannels(uint16_t (&channelValues)[2], const uint16_t value, const uint16_t &min, const uint16_t &max);
static void calibrateLimits(uint16_t (&limits)[2], const uint16_t &value, const uint16_t min, const uint16_t max);

void Ds600Rx::begin() const
{
    // ensure non-zero as actual zero in the center
    // a pull-down 6-7k(10k) resistor on each pin is required
    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    pinMode(CH3, INPUT);
    pinMode(CH4, INPUT);
    // note: unused
    // pinMode(CH5, INPUT);
    // pinMode(CH6, INPUT);
}

const RxOutput Ds600Rx::tryRead()
{
    updateChannels(
        this->channels[CH_STEERING],
        pulseIn(CH1, HIGH, 5000),
        steeringLimits[LIM_MIN],
        steeringLimits[LIM_MAX]);
    updateChannels(
        this->channels[CH_THROTTLE],
        pulseIn(CH2, HIGH, 5000),
        throttleLimits[LIM_MIN],
        throttleLimits[LIM_MAX]);
    updateChannels(
        this->channels[CH_ARMING],
        pulseIn(CH3, HIGH, 5000),
        DS600RX_ARMING_MIN,
        DS600RX_ARMING_MAX);
    updateChannels(
        this->channels[CH_CALIBRATING],
        pulseIn(CH4, HIGH, 5000),
        DS600RX_CALIBRATING_MIN,
        DS600RX_CALIBRATING_MAX);

    if (!this->linked && this->isConnected())
        this->linked = true;

    if (this->linked && this->armed != this->isArmed())
    {
        this->armed = !this->armed;
        Debug::logLn(this->armed ? "DS600RX ARMED" : "DS600RX DISARMED");
    }

    if (this->linked && !this->armed && this->isCalibrating())
    {
        calibrateLimits(steeringLimits, this->channels[CH_STEERING][CH_VALUE_CURR], DS600RX_STEERING_MIN, DS600RX_STEERING_MAX);
        calibrateLimits(throttleLimits, this->channels[CH_THROTTLE][CH_VALUE_CURR], DS600RX_THROTTLE_MIN, DS600RX_THROTTLE_MAX);
    }

    return this->linked
               ? RxOutput(this->getSteering(), this->getThrottle())
               : RxOutput::unlinked;
}

float Ds600Rx::getSteering() const
{
    if (!this->armed)
        return 0.0;

    auto value = mapNumber(this->channels[CH_STEERING][CH_VALUE_CURR], steeringLimits[LIM_MIN], steeringLimits[LIM_MAX], -1.0f, 1.0f);
    if (value < DS600RX_VALUE_EPSILON && value > -DS600RX_VALUE_EPSILON)
        return 0.0; // value is within range of a measurement error

    return value;
}

float Ds600Rx::getThrottle() const
{
    if (!this->armed)
        return 0.0;

    auto value = mapNumber(this->channels[CH_THROTTLE][CH_VALUE_CURR], throttleLimits[LIM_MIN], throttleLimits[LIM_MAX], -1.0f, 1.0f);
    if (value < DS600RX_VALUE_EPSILON && value > -DS600RX_VALUE_EPSILON)
        return 0.0; // value is within range of a measurement error

    return value;
}

bool Ds600Rx::isArmed() const
{
    return this->channels[CH_ARMING][CH_VALUE_CURR] >= DS900RX_AUX_MIN;
}

bool Ds600Rx::isCalibrating() const
{
    return this->channels[CH_CALIBRATING][CH_VALUE_CURR] >= DS900RX_AUX_MIN;
}

bool Ds600Rx::isConnected() const
{
    return this->channels[CH_STEERING][CH_VALUE_CURR] > 0 && this->channels[CH_THROTTLE][CH_VALUE_CURR] > 0 && this->channels[CH_ARMING][CH_VALUE_CURR] > 0;
}

static void updateChannels(uint16_t (&channelValues)[2], const uint16_t value, const uint16_t &min, const uint16_t &max)
{
    auto previousValue = channelValues[CH_VALUE_PREV];

    // big value change is ignored as measurement error unless repeatable
    if (abs((float)previousValue - (float)value) / previousValue > DS600RX_VALUE_EPSILON)
    {
        // update previous and keep current
        channelValues[CH_VALUE_PREV] = value;
    }
    else
    {
        // update previous and current
        channelValues[CH_VALUE_PREV] = value;
        channelValues[CH_VALUE_CURR] = value;
    }
}

static void calibrateLimits(uint16_t (&limits)[2], const uint16_t &value, const uint16_t min, const uint16_t max)
{
    // accept new edge values in 25% zone of original scale ignoring original edge values.
    if (value < 0.75 * min + 0.25 * max && (value < limits[LIM_MIN] || min == limits[LIM_MIN]))
        limits[LIM_MIN] = value;
    if (value > 0.25 * min + 0.75 * max && (value > limits[LIM_MAX] || max == limits[LIM_MIN]))
        limits[LIM_MAX] = value;
}
