#ifndef FILLED_CIRCLE_HPP
#define FILLED_CIRCLE_HPP

#include "effect.hpp"

class CircleEffect : public Effect
{
    Point m_center = Point(10,10);
    float m_radius = 5;
    Color3 m_color;
    bool m_fill;

public:
    CircleEffect(const& Point center, const float radius, const Color3& color, const bool fill)
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};

#endif
