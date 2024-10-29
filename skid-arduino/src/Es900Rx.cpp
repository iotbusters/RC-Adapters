#include <Es900Rx.h>
#include <Debug.h>
#include <Utils.h>
#include <crc8.h>

static float mapFloat(int value, int min, int max);

void Es900Rx::begin()
{
    this->crossfire.begin(&this->serial, 115200);
}

const bool Es900Rx::tryRead()
{
    if (!this->serial.isListening())
        this->serial.listen();

    this->crossfire.readPacket();

    if (this->linked != this->crossfire.isConnected())
    {
        this->linked = !this->linked;
        Debug::logLn(this->linked ? "ES900RX LINKED" : "ES900RX UNLINKED");
    }

    if (this->armed != this->isArmed())
    {
        this->armed = !this->armed;
        Debug::logLn(this->armed ? "ES900RX ARMED" : "ES900RX DISARMED");
    }

    return this->linked;
}

RxOutput Es900Rx::getOutput() const
{
    return this->linked
               ? RxOutput(this->getSteering(), this->getThrottle())
               : RxOutput::unlinked;
}

float Es900Rx::getSteering() const
{
    if (!this->armed)
        return 0.0;

    return mapFloat(this->crossfire.getChannel(1), ES900RX_STEERING_MIN, ES900RX_STEERING_MAX);
}

float Es900Rx::getThrottle() const
{
    if (!this->armed)
        return 0.0;

    return mapFloat(this->crossfire.getChannel(2), ES900RX_THROTTLE_MIN, ES900RX_THROTTLE_MAX);
}

bool Es900Rx::isArmed() const
{
    return this->crossfire.getChannel(5) > ES900RX_AUX_MIN;
}

static float mapFloat(int value, int min, int max)
{
    value = compareExchange(value, (min + max) / 2, 0);
    return mapNumber(value, min, max, -1.0f, 1.0f);
}
