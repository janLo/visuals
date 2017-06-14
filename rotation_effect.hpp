#ifndef ROTATION_EFFECT_HPP
#define ROTATION_EFFECT_HPP


#include "effect.hpp"

class RotationEffect : public Effect
{
public:
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};



#endif
