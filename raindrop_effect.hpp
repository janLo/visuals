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

    bool draw(EffectBuffer& buffer, const unsigned int height, const unsigned int width, const double time) const;
    void reset(const double time);

};

class EffectRaindrops : public Effect {
    unsigned int m_height;
    unsigned int m_width;
    std::vector<Raindrop> drops;

public:

    EffectRaindrops(unsigned int height, unsigned int width, double time);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
    ~EffectRaindrops() {}
};


#endif
