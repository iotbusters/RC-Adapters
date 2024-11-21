#pragma once

#include <Arduino.h>

#define guard(e) ((e) ? (void)0 : abort())

#define UINT10_VALUE_MIN 0
#define UINT10_VALUE_MAX 1023
#define UINT12_VALUE_MIN 0
#define UINT12_VALUE_MAX 4095
#define FRACT_VALUE_MIN -1.0f
#define FRACT_VALUE_ZERO 0.0f
#define FRACT_VALUE_MAX 1.0f
#define UFRACT_VALUE_MIN 0.0f
#define UFRACT_VALUE_MAX 1.0f

#define EPSILON_FLOAT 0.001f // considered as a smallest number with floating point

template <typename... Args>
void print(const char *format, Args... args) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), format, args...);
    Serial.print(buffer);
    delay(50); // time for serial
}

template <typename... Args>
void println(const char *format, Args... args) {
    print(format, args...);
    Serial.println();
}

template <typename TIn, typename TOut>
TOut mapNumber(const TIn x, const TIn in_min, const TIn in_max, const TOut out_min, const TOut out_max) {
    guard(in_min < in_max);
    guard(out_min < out_max);
    if (x < in_min)
        return out_min;
    if (x > in_max)
        return out_max;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

struct ufract_t;
struct uint12_t;

template <>
uint12_t mapNumber<ufract_t, uint12_t>(const ufract_t x, const ufract_t in_min, const ufract_t in_max, const uint12_t out_min,
                                       const uint12_t out_max);

template <typename T, byte N>
T averageNumber(const T (&array)[N]) {
    static_assert(N > 0, "Array must contain at least one element");
    T sum = 0;
    for (byte i = 0; i < N; i++)
        sum += array[i];
    return sum / N;
}

template <typename T, byte N>
T minArray(const T (&array)[N]) {
    static_assert(N > 0, "Array must contain at least one element");
    T min = array[0];
    for (byte i = 1; i < N; i++)
        if (min > array[i])
            min = array[i];
    return min;
}

template <typename T>
T compareExchange(const T value, const T exchangeValue, const T comparand) {
    return abs(value - comparand) < EPSILON_FLOAT ? exchangeValue : value;
}

template <typename T, byte N>
constexpr byte size(const T (&)[N]) {
    return N;
}

typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

struct ufract_t {
private:
    const float value;

    static float ensure(const float value) {
        return value < UFRACT_VALUE_MIN ? UFRACT_VALUE_MIN : value > UFRACT_VALUE_MAX ? UFRACT_VALUE_MAX : value;
    }

public:
    ufract_t(const float value) : value(ensure(value)) {}
    ufract_t(const double value) : value(ensure(value)) {}

    void operator=(const ufract_t &value) const { value = this->value; }

    // Arithmetic operators
    ufract_t operator-() const { return ufract_t(-value); }
    ufract_t operator+(const ufract_t &r) const { return ufract_t(value + r.value); }
    ufract_t operator-(const ufract_t &r) const { return ufract_t(value - r.value); }
    ufract_t operator*(const ufract_t &r) const { return ufract_t(value * r.value); }
    ufract_t operator/(const ufract_t &r) const { return r.value != 0.0f ? value / r.value : UFRACT_VALUE_MAX; }

    // Comparison operators
    bool operator==(const ufract_t &r) const { return value == r.value; }
    bool operator!=(const ufract_t &r) const { return value != r.value; }
    bool operator<(const ufract_t &r) const { return value < r.value; }
    bool operator>(const ufract_t &r) const { return value > r.value; }
    bool operator<=(const ufract_t &r) const { return value <= r.value; }
    bool operator>=(const ufract_t &r) const { return value >= r.value; }

    // Conversion operators
    operator float() const { return value; }
    operator double() const { return static_cast<double>(value); }
};

struct fract_t {
private:
    const float value;

    static float ensure(const float value) {
        return value < FRACT_VALUE_MIN ? FRACT_VALUE_MIN : value > FRACT_VALUE_MAX ? FRACT_VALUE_MAX : value;
    }

public:
    fract_t(const float value) : value(ensure(value)) {}
    fract_t(const double value) : value(ensure(value)) {}
    fract_t(const ufract_t value) : value((float)value) {}

    ufract_t absolute() const { return abs(value); }
    fract_t sign(const signed char sign) const { return value * sign > 0 ? *this : -(*this); }

    void operator=(const fract_t &value) const { value = this->value; }

    // Arithmetic operators
    fract_t operator-() const { return fract_t(-value); }
    fract_t operator+(const fract_t &r) const { return fract_t(value + r.value); }
    fract_t operator-(const fract_t &r) const { return fract_t(value - r.value); }
    fract_t operator*(const fract_t &r) const { return fract_t(value * r.value); }
    fract_t operator/(const fract_t &r) const { return r.value != 0.0f ? value / r.value : FRACT_VALUE_MAX; }

    // Comparison operators
    bool operator==(const fract_t &r) const { return value == r.value; }
    bool operator!=(const fract_t &r) const { return value != r.value; }
    bool operator<(const fract_t &r) const { return value < r.value; }
    bool operator>(const fract_t &r) const { return value > r.value; }
    bool operator<=(const fract_t &r) const { return value <= r.value; }
    bool operator>=(const fract_t &r) const { return value >= r.value; }

    // Conversion operators
    operator float() const { return value; }
    operator double() const { return static_cast<double>(value); }
};

struct uint10_t {
private:
    const uint16_t value;

public:
    uint10_t(const uint8_t value) : value(value) {}
    uint10_t(const uint16_t value) : value(value > UINT10_VALUE_MAX ? UINT10_VALUE_MAX : value) {}
    uint10_t(const int value) : value(value > UINT10_VALUE_MAX ? UINT10_VALUE_MAX : value) {}

    void operator=(const uint10_t &value) const { value = this->value; }

    // Arithmetic operators
    uint10_t operator+(const uint10_t &i) const { return uint10_t(value + i.value); }
    uint10_t operator-(const uint10_t &i) const { return uint10_t(value > i.value ? value - i.value : 0); }
    uint10_t operator*(const uint10_t &i) const { return uint10_t(value * i.value); }
    uint10_t operator/(const uint10_t &i) const { return i.value != 0 ? value / i.value : UINT10_VALUE_MAX; }

    // Comparison operators
    bool operator==(const uint10_t &i) const { return value == i.value; }
    bool operator!=(const uint10_t &i) const { return value != i.value; }
    bool operator<(const uint10_t &i) const { return value < i.value; }
    bool operator>(const uint10_t &i) const { return value > i.value; }
    bool operator<=(const uint10_t &i) const { return value <= i.value; }
    bool operator>=(const uint10_t &i) const { return value >= i.value; }

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

struct uint12_t {
private:
    const uint16_t value;

public:
    uint12_t(const uint8_t value) : value(value) {}
    uint12_t(const uint16_t value) : value(value > UINT12_VALUE_MAX ? UINT12_VALUE_MAX : value) {}
    uint12_t(const int value) : value(value > UINT12_VALUE_MAX ? UINT12_VALUE_MAX : value) {}

    void operator=(const uint12_t &value) const { value = this->value; }

    // Arithmetic operators
    uint12_t operator+(const uint12_t &i) const { return uint12_t(value + i.value); }
    uint12_t operator-(const uint12_t &i) const { return uint12_t(value > i.value ? value - i.value : 0); }
    uint12_t operator*(const uint12_t &i) const { return uint12_t(value * i.value); }
    uint12_t operator/(const uint12_t &i) const { return i.value != 0 ? value / i.value : UINT12_VALUE_MAX; }

    // Comparison operators
    bool operator==(const uint12_t &i) const { return value == i.value; }
    bool operator!=(const uint12_t &i) const { return value != i.value; }
    bool operator<(const uint12_t &i) const { return value < i.value; }
    bool operator>(const uint12_t &i) const { return value > i.value; }
    bool operator<=(const uint12_t &i) const { return value <= i.value; }
    bool operator>=(const uint12_t &i) const { return value >= i.value; }

    // Conversion operators
    operator uint8_t() const { return static_cast<uint8_t>(value); }
    operator uint16_t() const { return static_cast<uint16_t>(value); }
    operator int() const { return static_cast<int>(value); }
};

#define UINT10_MIN uint10_t(UINT10_VALUE_MIN)
#define UINT10_MAX uint10_t(UINT10_VALUE_MAX)
#define UINT12_MIN uint12_t(UINT12_VALUE_MIN)
#define UINT12_MAX uint12_t(UINT12_VALUE_MAX)
#define FRACT_MIN fract_t(FRACT_VALUE_MIN)
#define FRACT_ZERO fract_t(FRACT_VALUE_ZERO)
#define FRACT_MAX fract_t(FRACT_VALUE_MAX)
#define UFRACT_MIN ufract_t(UFRACT_VALUE_MIN)
#define UFRACT_MAX ufract_t(UFRACT_VALUE_MAX)
