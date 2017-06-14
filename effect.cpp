#include "effect.hpp"
#include "color.hpp"

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
