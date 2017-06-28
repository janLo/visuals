#ifndef FILL_EFFECT_HPP
#define FILL_EFFECT_HPP

#include "effect.hpp"
#include "color_maker.hpp"

class FillEffect : public Effect
{
    std::shared_ptr<ColorMaker> m_color_maker;
    double m_onset;
    float m_last;
public:
    FillEffect(std::shared_ptr<ColorMaker> color_maker): m_color_maker(color_maker) { }
    void fill(EffectBuffer& buffer, const EffectState& state);
};

#endif
