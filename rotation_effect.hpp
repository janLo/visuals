#ifndef ROTATION_EFFECT_HPP
#define ROTATION_EFFECT_HPP


#include "effect.hpp"

class RotationEffect : public Effect
{

    unsigned int m_width;
    unsigned int m_height;

public:
    RotationEffect(unsigned int width, unsigned int height);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};



#endif
