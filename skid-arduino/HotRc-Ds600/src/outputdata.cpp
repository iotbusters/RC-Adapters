#include <outputdata.h>

OutputData::OutputData() : speed2(false), speed3(false), throttle(0), isReversed(false) {}

OutputData::OutputData(bool speed2, bool speed3, float throttle, bool isReversed)
{
    this->speed2 = speed2;
    this->speed3 = speed3;
    this->throttle = throttle;
    this->isReversed = isReversed;
}

bool OutputData::Breaks() { return *this == OutputData::idle; }

bool OutputData::operator==(const OutputData &other) const
{
    return speed2 == other.speed2 && speed3 == other.speed3 && throttle == other.throttle && isReversed == other.isReversed;
}

bool OutputData::operator!=(const OutputData &other) const
{
    return !(*this == other);
}

OutputData OutputData::idle;