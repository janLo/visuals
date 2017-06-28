#ifndef FILL_EFFECT_HPP
#define FILL_EFFECT_HPP

#include "effect.hpp"

class FillEffect : public Effect
{
    Color3 m_color;
    double m_onset;
    float m_last;
public:
    FillEffect(const Color3& color): m_color(color) { }
    void fill(EffectBuffer& buffer, const EffectState& state);
};

#endif
