#include <RxOutput.h>

const RxOutput &RxOutput::operator||(const RxOutput &other) const
{
    return this->isLinked ? *this : other;
}

const RxOutput RxOutput::unlinked;
