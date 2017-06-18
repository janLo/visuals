#ifndef OCEAN_EFFECT_HPP
#define OCEAN_EFFECT_HPP

#include "effect.hpp"


class OceanEffect : public Effect
{
public:
    OceanEffect();
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};

#endif
