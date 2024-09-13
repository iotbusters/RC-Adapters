#pragma once

#include <Arduino.h>
#include <Assert.h>

#define INT_12BIT_MIN 0
#define INT_12BIT_MAX 4095

#define EPSILON_FLOAT 0.001f // considered as a smallest number with floating point

template <typename TIn, typename TOut>
TOut mapNumber(const TIn x, const TIn in_min, const TIn in_max, const TOut out_min, const TOut out_max)
{
    assert(in_min < in_max);
    assert(out_min < out_max);
    if (x < in_min)
        return out_min;
    if (x > in_max)
        return out_max;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T, byte N>
T numberAverage(const T (&array)[N])
{
    static_assert(N > 0, "Array must contain at least one element");
    T sum = 0;
    for (byte i = 0; i < N; i++)
        sum += array[i];
    return sum / N;
}

template <typename T, byte N>
T minArray(const T (&array)[N])
{
    static_assert(N > 0, "Array must contain at least one element");
    T min = array[0];
    for (byte i = 1; i < N; i++)
        if (min > array[i])
            min = array[i];
    return min;
}

template <typename T>
T compareExchange(const T value, const T exchangeValue, const T comparand)
{
    return abs(value - comparand) < EPSILON_FLOAT ? exchangeValue : value;
}

template <typename T, byte N>
constexpr byte size(const T (&)[N]) { return N; }
