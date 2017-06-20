#include "effect.hpp"
#include "color.hpp"
#include "color_utils.hpp"

AddEffect::AddEffect(std::shared_ptr<Effect> base, const float coeff)
: m_base(base), m_coeff(coeff)
{}

void AddEffect::fill(EffectBuffer& buffer, const EffectState& state) {
    EffectBuffer temp;
    m_base->fill(temp, state);
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] += temp[i] * m_coeff;
    }
}

AddEffect::~AddEffect() {}


Color3 RandomColorMaker::make(const EffectState& state)
{
return HSVtoRGB(Color3(rand()/(float)RAND_MAX, 1.0f, 0.5f));
}
