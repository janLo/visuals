#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include "color.hpp"
#include <algorithm>
#include <cmath>


float inline dot(float x1, float y1, float x2, float y2)
{
    return sqrt(x1*x2 + y1*y2);
}

float inline length(float x, float y)
{
    return sqrt(x*x + y*y);
}


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
Color3 RGBtoHSV(Color3 rgb);

#endif
