#ifndef RAINDROP_EFFECT_HPP
#define RAINDROP_EFFECT_HPP

#include "effect.hpp"
#include "color.hpp"

class Raindrop
{
    unsigned int m_col;
    double m_speed;
    double m_start;
    Color3 m_color;

public:

    Raindrop(unsigned int col, double start, Color3 color);

    bool draw(EffectBuffer& buffer, const double time) const;
    void reset(const double time);

};

class RaindropEffect : public Effect {
    std::vector<Raindrop> drops;

public:

    RaindropEffect(unsigned int width, double time);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
    ~RaindropEffect() {}
};


#endif
