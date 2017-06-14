#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include "color.hpp"
#include <algorithm>
#include <cmath>

template<class T>
inline T fmod2(T x, T y)
{
    if (x >= T(0))
        return std::fmod(x, y);
    return std::fmod(x, y) + y;
}

inline Color3 saturate(const Color3& col)
{
    return Color3(
        std::max(0.0f, std::min(1.0f, col.r)),
        std::max(0.0f, std::min(1.0f, col.g)),
        std::max(0.0f, std::min(1.0f, col.b))
        );
}

//Color3 hue(float h);
//Color3 HSVtoRGB(const Color3& hsv);
Color3 HSVtoRGB(const Color3& hsv);

#endif
