#ifndef PLASMA_EFFECT_HPP
#define PLASMA_EFFECT_HPP

#include "effect.hpp"


class PlasmaEffect : public Effect {
    const unsigned int m_height;
    const unsigned int m_width;

public:
    PlasmaEffect(const unsigned int hight, const unsigned int width);
    void fill(EffectBuffer& buffer, const EffectState& state) override;
};


#endif
