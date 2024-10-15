#include <Es900Rx.h>
#include <Debug.h>
#include <Utils.h>
#include <crc8.h>

auto isLinked = false;

static float map(int value, int min, int max)
{
    value = compareExchange(value, (min + max) / 2, 0);
    return mapNumber(value, min, max, -1.0f, 1.0f);
}

void Es900Rx::begin()
{
    this->crossfire.begin(&this->serial, 115200);
}

bool Es900Rx::tryLink()
{
    if (!this->serial.isListening())
        this->serial.listen();

    this->crossfire.readPacket();

    if (isLinked != this->crossfire.isConnected())
    {
        isLinked = !isLinked;
        Debug::logLn(isLinked ? "Es900Rx linked" : "Es900Rx unlinked");
    }

    return this->crossfire.isConnected();
}

float Es900Rx::channel1()
{
    return map(this->crossfire.getChannel(1), ES900RX_STEERING_MIN, ES900RX_STEERING_MAX);
}

float Es900Rx::channel2()
{
    return map(this->crossfire.getChannel(2), ES900RX_THROTTLE_MIN, ES900RX_THROTTLE_MAX);
}
