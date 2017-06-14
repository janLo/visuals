#include "color_utils.hpp"

/*
Color3 hue(float h)
{
    float r = abs(h * 6 - 3) - 1;
    float g = 2 - abs(h * 6 - 2);
    float b = 2 - abs(h * 6 - 4);
    return saturate(Color3(r, g, b));
}

Color3 HSVtoRGB(const Color3& hsv)
{
    return Color3(((hue(hsv.r) - 1.0f) * hsv.g + 1.0f) * hsv.b);
}
*/
Color3 HSVtoRGB(const Color3& hsv)
{
    Color3 out;
    float h = hsv.r;
    float s = hsv.g;
    float v = hsv.b;
    if (s == 0.0f)
    {
        // gray
        out.r = out.g = out.b = v;
        return out;
    }

    h = fmodf(h, 1.0f) / (60.0f/360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out.r = v; out.g = t; out.b = p; break;
    case 1: out.r = q; out.g = v; out.b = p; break;
    case 2: out.r = p; out.g = v; out.b = t; break;
    case 3: out.r = p; out.g = q; out.b = v; break;
    case 4: out.r = t; out.g = p; out.b = v; break;
    case 5: default: out.r = v; out.g = p; out.b = q; break;
    }
    return out;
}
