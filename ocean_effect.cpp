
#include "ocean_effect.hpp"
#include <algorithm>
#include <cmath>
#include "color_utils.hpp"

OceanEffect::OceanEffect()
{}

int line = 5;

float value(float x, float time)
{
    float v = sin(x / 25.0f * 6) * 1.2f + sin(x / 25.0f * 2 + time * 1.7f) * 1.7f;
    return v;
}

void OceanEffect::fill(EffectBuffer& buffer, const EffectState& state) {
    for (int x=0; x<buffer.width(); x++)
        for (int y=0; y<buffer.height() - line + value(x, state.time + state.rotation.x); y++)
            buffer.set(x, y, HSVtoRGB(Color3(20.0f/360.0f - y/360.0f, 1.0f, (float)y/line/10.0f)));

    for (int x=0; x<buffer.width(); x++)
        for (int y=buffer.height() - line + value(x, state.time + state.rotation.x); y < buffer.height(); y++)
            buffer.set(x, y, Color3(2/100.0f, 4/100.0f, 8/100.0f)); //HSVtoRGB(Color3(256.0f / 360.0f, 0.6f, 0.2f)));

}
