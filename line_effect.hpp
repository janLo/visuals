#ifndef LINE_EFFECT_HPP
#define LINE_EFFECT_HPP

#include "effect.hpp"
#include "color.hpp"


class LineEffect : public Effect
{

    const Point m_p1;
    const Point m_p2;
    const Color3 m_color;

public:
    LineEffect(const Point& p1, const Point& p2, const Color3 color);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};


#endif
