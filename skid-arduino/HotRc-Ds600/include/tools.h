template <typename T>
T fmap(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T, uint8_t N>
T avg(const T (&array)[N])
{
    T sum = 0;
    for (uint8_t i = 0; i < N; i++)
        sum += array[i];
    return sum / N;
}

template <typename T, uint8_t N>
constexpr uint8_t size(const T (&)[N]) { return N; }
