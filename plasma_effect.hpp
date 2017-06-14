#ifndef PLASMA_EFFECT_HPP
#define PLASMA_EFFECT_HPP

#include "effect.hpp"


class PlasmaEffect : public Effect {
public:
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};


#endif
