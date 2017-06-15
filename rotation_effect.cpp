#include <iostream>

#include "rotation_effect.hpp"
#include "color.hpp"
#include "color_utils.hpp"

void RotationEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    float rot = fmod2(state.rotation.x / 3.141592f / 2.0f, 1.0f);
#ifdef DEBUG
    std::cout << state.rotation.x << " " << " " << state.rotation.y << " " << " " << state.rotation.z << " " << rot << std::endl;
#endif
    EffectBuffer buf2;
    for (int x=0; x<buffer.width(); x++) {
	const unsigned int column_1 = (x + size_t(rot * buffer.width()) + 0) % buffer.width();
	const unsigned int column_2 = (x + size_t(rot * buffer.width()) + 1) % buffer.width();

        for (int y=0; y<buffer.height(); y++) {
            float frac = fmod(rot * buffer.width(), 1.0f);
            auto col1 = buffer.get(column_1, y);
            auto col2 = buffer.get(column_2, y);
            buf2.set(x, y, col1 * (1 - frac) + col2 * frac);
        }
    }

    buffer = buf2;
}
