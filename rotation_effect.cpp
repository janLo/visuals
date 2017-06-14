#include "rotation_effect.hpp"
#include "color.hpp"
#include "color_utils.hpp"


RotationEffect::RotationEffect(unsigned int width, unsigned int height)
: m_width(width), m_height(height)
{}


void RotationEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    float rot = fmod2(state.rotation / 3.141592f / 2.0f, 1.0f);
    //std::cout << m_x << " " << " " << m_y << " " << " " << m_z << " " << rot << std::endl;
    std::vector<unsigned int> buf2(buffer.size());
    for (int x=0; x<m_width; x++) {
        for (int y=0; y<m_height; y++) {
            float frac = fmod(rot * m_width, 1.0f);
            Color3 col1(buffer[y * m_width + (x + size_t(rot * m_width) + 0) % m_width]);
            Color3 col2(buffer[y * m_width + (x + size_t(rot * m_width) + 1) % m_width]);
            buf2[y * m_width + x] = /*col1 + (col2 - col1) * frac;*/ col1 * (1 - frac) + col2 * frac;
        }
    }

    std::copy(buf2.begin(), buf2.end(), buffer.begin());
}
