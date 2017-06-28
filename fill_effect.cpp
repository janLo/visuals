#include "fill_effect.hpp"


void FillEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    float current = state.sound.kick;

    if (current > 0.3f) {
        m_onset = state.time;
    } else {
        if (m_last > 0.1) {
            current = m_last * 0.8;
        }
    }
    m_last = current;
    Color3 color = m_color * (0.5f + (current * 0.5));
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = color;
    }
}

