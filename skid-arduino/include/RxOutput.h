#pragma once

#include <Arduino.h>

struct RxOutput
{
private:
    RxOutput() : isLinked(false), steering(0.0), throttle(0.0) {}

public:
    static const RxOutput unlinked;

    const bool isLinked;
    const float steering;
    const float throttle;

    RxOutput(float steering, float throttle) : isLinked(true), steering(steering), throttle(throttle) {}

    const RxOutput &operator||(const RxOutput &other) const;
};
