#include <Utils.h>

template <>
uint12_t mapNumber<ufract_t, uint12_t>(const ufract_t x, const ufract_t in_min, const ufract_t in_max, const uint12_t out_min,
                                       const uint12_t out_max) {
    guard(in_min < in_max);
    guard(out_min < out_max);
    if (x < in_min)
        return out_min;
    if (x > in_max)
        return out_max;
    auto in_range_pos = (float)x - (float)in_min;
    auto in_range = (float)in_max - (float)in_min;
    auto out_range = (uint16_t)out_max - (uint16_t)out_min;
    return (uint16_t)((in_range_pos * out_range) / in_range + (uint16_t)out_min);
}
