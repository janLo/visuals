#ifndef WAVE_EFFECT_HPP
#define WAVE_EFFECT_HPP

#include "effect.hpp"


class TopDownWaveEffect : public Effect
{
    double m_start = -100;
    float m_duration = 1;

public:
    TopDownWaveEffect(float duration);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};

#endif
