#pragma once

#include <Arduino.h>
#include <Assert.h>

#define UINT10_MAX 1023
#define UINT12_MAX 4095

#define RATIO_MIN 0.0
#define RATIO_MAX 1.0

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
T averageNumber(const T (&array)[N])
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

typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

struct ratio_t
{
private:
    const float value;

    static float ensure(float value)
    {
        if (value < RATIO_MIN)
            return RATIO_MIN;
        else if (value > RATIO_MAX)
            return RATIO_MAX;
        else
            return value;
    }

public:
    ratio_t(uint8_t value) : value(ensure(value)) {}
    ratio_t(uint16_t value) : value(ensure(value)) {}
    ratio_t(int value) : value(ensure(value)) {}
    ratio_t(float value) : value(ensure(value)) {}

    // Arithmetic operators
    ratio_t operator+(const ratio_t &r) { return ratio_t(value + r.value); }
    ratio_t operator-(const ratio_t &r) { return ratio_t(value - r.value); }
    ratio_t operator*(const ratio_t &r) { return ratio_t(value * r.value); }
    ratio_t operator/(const ratio_t &r) { return r.value != 0.0f ? ratio_t(value / r.value) : ratio_t(value); }

    // Comparison operators
    bool operator==(const ratio_t &r) { return value == r.value; }
    bool operator!=(const ratio_t &r) { return value != r.value; }
    bool operator<(const ratio_t &r) { return value < r.value; }
    bool operator>(const ratio_t &r) { return value > r.value; }
    bool operator<=(const ratio_t &r) { return value <= r.value; }
    bool operator>=(const ratio_t &r) { return value >= r.value; }

    // Conversion operators
    operator bool() const { return static_cast<bool>(value); }
    operator uint8_t() const { return static_cast<uint8_t>(value); }
    operator uint16_t() const { return static_cast<uint16_t>(value); }
    operator int() const { return static_cast<int>(value); }
    operator float() const { return value; }
    operator double() const { return static_cast<double>(value); }
};

struct uint10_t
{
private:
    const uint16_t value;

public:
    uint10_t(uint8_t value) : value(value) {}
    uint10_t(int value) : value(value > UINT10_MAX ? UINT10_MAX : value) {}
    uint10_t(uint16_t value) : value(value > UINT10_MAX ? UINT10_MAX : value) {}

    // Arithmetic operators
    uint10_t operator+(const uint10_t &i) { return uint10_t(value + i.value); }
    uint10_t operator-(const uint10_t &i) { return uint10_t(value > i.value ? value - i.value : 0); }
    uint10_t operator*(const uint10_t &i) { return uint10_t(value * i.value); }
    uint10_t operator/(const uint10_t &i) { return i.value != 0 ? uint10_t(value / i.value) : uint10_t(value); }

    // Comparison operators
    bool operator==(const uint10_t &i) { return value == i.value; }
    bool operator!=(const uint10_t &i) { return value != i.value; }
    bool operator<(const uint10_t &i) { return value < i.value; }
    bool operator>(const uint10_t &i) { return value > i.value; }
    bool operator<=(const uint10_t &i) { return value <= i.value; }
    bool operator>=(const uint10_t &i) { return value >= i.value; }

    // Bitwise operators
    uint10_t operator>>(uint8_t shift) const { return uint10_t(value >> shift); }
    uint10_t operator<<(uint8_t shift) const { return uint10_t(value << shift); }
    uint10_t operator&(uint10_t mask) const { return uint10_t(value & mask.value); }
    uint10_t operator|(uint10_t mask) const { return uint10_t(value | mask.value); }

    // Conversion operators
    operator uint8_t() const { return static_cast<uint8_t>(value); }
    operator uint16_t() const { return static_cast<uint16_t>(value); }
    operator int() const { return static_cast<int>(value); }
};

struct uint12_t
{
private:
    uint16_t value;

public:
    uint12_t(uint8_t value) : value(value) {}
    uint12_t(int value) : value(value > UINT12_MAX ? UINT12_MAX : value) {}
    uint12_t(uint16_t value) : value(value > UINT12_MAX ? UINT12_MAX : value) {}

    // Arithmetic operators
    uint12_t operator+(const uint12_t &i) { return uint12_t(value + i.value); }
    uint12_t operator-(const uint12_t &i) { return uint12_t(value > i.value ? value - i.value : 0); }
    uint12_t operator*(const uint12_t &i) { return uint12_t(value * i.value); }
    uint12_t operator/(const uint12_t &i) { return i.value != 0 ? uint12_t(value / i.value) : uint12_t(value); }

    // Comparison operators
    bool operator==(const uint12_t &i) { return value == i.value; }
    bool operator!=(const uint12_t &i) { return value != i.value; }
    bool operator<(const uint12_t &i) { return value < i.value; }
    bool operator>(const uint12_t &i) { return value > i.value; }
    bool operator<=(const uint12_t &i) { return value <= i.value; }
    bool operator>=(const uint12_t &i) { return value >= i.value; }

    // Conversion operators
    operator uint8_t() const { return static_cast<uint8_t>(value); }
    operator uint16_t() const { return static_cast<uint16_t>(value); }
    operator int() const { return static_cast<int>(value); }
};
