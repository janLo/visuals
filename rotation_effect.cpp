#include <iostream>

#include "rotation_effect.hpp"
#include "color.hpp"
#include "color_utils.hpp"

void RotationEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    float rot = fmod2(state.rotation.x / 3.141592f / 2.0f, 1.0f);
    std::cout << state.rotation.x << " " << " " << state.rotation.y << " " << " " << state.rotation.z << " " << rot << std::endl;
    EffectBuffer buf2;
    for (int x=0; x<buffer.width(); x++) {
        for (int y=0; y<buffer.height(); y++) {
            float frac = fmod(rot * buffer.width(), 1.0f);
            auto col1 = buffer.get((x + size_t(rot * buffer.width()) + 0) % buffer.width(), y);
            auto col2 = buffer.get((x + size_t(rot * buffer.width()) + 1) % buffer.width(), y);
            buf2.set(x, y, col1 * (1 - frac) + col2 * frac);
        }
    }

    buffer = buf2;
}
