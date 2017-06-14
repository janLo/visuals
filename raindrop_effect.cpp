#include "raindrop_effect.hpp"
#include "color_utils.hpp"
#include <cmath>



Raindrop::Raindrop(unsigned int col, double start, Color3 color)
    : m_col(col), m_speed(1 + (rand() % 7) ), m_start(start), m_color(color)
{
}

bool Raindrop::draw(EffectBuffer& buffer, const double time) const
{
    double elapsed = time - m_start;

    if (0 > elapsed) {
        return true;
    }

    double step_time = m_speed / buffer.height();
    unsigned int length = buffer.height() / std::min(2.0, m_speed*0.5f);

    double offset = (time - m_start) / step_time;
    double step = std::floor(offset);

    if ((step - length) > buffer.height()) {
        return false;
    }

    for (int pos = length - 1; pos >= 0; --pos) {
        int position = offset - pos;
        if (position < 0 || position >= buffer.height()) {
            continue;
        }
        buffer.set(m_col, position, m_color * float(((float(length - pos)) / length) * std::min(1.0f, (float(length) /  (1.5f * pos)))));
    }
    return true;
}

void Raindrop::reset(const double time)
{
    m_start = time;
    m_speed = 1 + rand() % 7;
}




RaindropEffect::RaindropEffect(unsigned int width, double time)
{
    srand(0);
    for (unsigned int col = 0; col < width; ++col) {
        Color3 color = HSVtoRGB(Color3(col / 50.0f, 1.0f, 1.0f));
        drops.push_back(Raindrop(col, time + (rand() % 25) / 10.0f, color));
    }
}

void RaindropEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    for (auto& drop : drops) {
        if (!drop.draw(buffer, state.time)) {
            drop.reset(state.time + (rand() % 50) / 10.0f);
        }
    }
}
