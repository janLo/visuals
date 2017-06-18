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

Color3 RGBtoHSV(Color3 rgb)
{
    rgb.r *= 255.0f;
    rgb.g *= 255.0f;
    rgb.b *= 255.0f;

    Color3 hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.b = rgbMax;
    if (hsv.b == 0)
    {
        hsv.r = 0;
        hsv.g = 0;
        return hsv;
    }

    hsv.g = 255 * long(rgbMax - rgbMin) / hsv.b;
    if (hsv.g == 0)
    {
        hsv.r = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.r = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.r = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.r = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    hsv.r /= 255.0f;
    hsv.g /= 255.0f;
    hsv.b /= 255.0f;

    return hsv;
}
