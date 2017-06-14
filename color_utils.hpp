#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include "color.hpp"
#include <cmath>

template<class T>
T fmod2(T x, T y)
{
    if (x >= T(0))
        return std::fmod(x, y);
    return std::fmod(x, y) + y;
}

Color3 saturate(const Color3& col);
//Color3 hue(float h);
//Color3 HSVtoRGB(const Color3& hsv);
Color3 HSVtoRGB(const Color3& hsv);

#endif
