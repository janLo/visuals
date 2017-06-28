#include "fill_effect.hpp"


void FillEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    Color3 color = m_color_maker->make(state);
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = color;
    }
}

