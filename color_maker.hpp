#ifndef COLOR_MAKER_HPP
#define COLOR_MAKER_HPP

#include "color.hpp"
#include "effect.hpp"

class ColorMaker
{
public:
    virtual Color3 make(const EffectState& state) = 0;
};


class ConstColorMaker : public ColorMaker
{
    Color3 m_color;
public:
    ConstColorMaker(const Color3& color) : m_color(color) {}
    Color3 make(const EffectState& state) override
    {
        return m_color;
    }
};


class RandomColorMaker : public ColorMaker
{
public:
    Color3 make(const EffectState& state) override;
};

class KickColorIntensity : public ColorMaker
{
    Color3 m_color;
    double m_onset_time;
    float m_last;
public:
    KickColorIntensity(const Color3& color) : m_color(color) {}
    Color3 make(const EffectState& state) override;
};

#endif
