#ifndef STARS_EFFECT_HPP
#define STARS_EFFECT_HPP

#include "effect.hpp"
#include "color_utils.hpp"

class StarsEffect : public Effect
{
    class Star {
    public:
        bool m_enabled = false;
        float m_time = 0.0f;
        int m_x, m_y;
        Color3 m_color;
    };

    std::vector<Star> m_stars;
    std::shared_ptr<ColorMaker> m_color_maker;
    Rect m_box;
public:
    StarsEffect(std::shared_ptr<ColorMaker> color_maker, const Rect& box, int num_stars = 10);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};

#endif
