#include "filled_circle_effect.hpp"
#include "color_utils.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>


CircleEffect::CircleEffect(const Point& center, const float radius, const Color3& color, const bool fill)
: m_center(center), m_radius(radius), m_color(color), m_fill(fill)
{}

void CircleEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    const int x_min = std::floor(m_center.x - m_radius) - 1;
    const int x_max = std::ceil(m_center.x + m_radius) + 1;

    const int y_min = std::max(0, int(std::floor(m_center.y - m_radius) - 1));
    const int y_max = std::min(buffer.height()-1, int(std::ceil(m_center.y + m_radius) + 1));

    const float rad_square = m_radius * m_radius;

    for (int y = y_min; y < y_max; ++y) {
        float b = float(y) - m_center.y;
        for (int x = x_min; x < x_max; ++x) {
            float a = float(x) - m_center.x;
            float d = std::sqrt(a*a + b*b) - m_radius;

            if (m_fill) {
                if (d < 1.0) {
                    float coeff = float(std::min(1.0f, 1.0f - d));
                    buffer.set(fmod2(x, buffer.width()), y,  m_color * coeff);
                }
            } else {
                if (d < 1.0 && d > -1.5) {
                  float coeff = (d > 0 ? 1.0f - d : 1.0f + d);
                   buffer.set(fmod2(x, buffer.width()), y, m_color * float(std::min(1.0f, coeff)));
                }
            }
        }
    }
}

ExtendingCircleEffect::ExtendingCircleEffect(float radius, const Color3& color, float duration, double time)
: m_radius(radius), m_duration(duration), m_start(time), circle(Point(0, 0), 0, color, false)
{}

void ExtendingCircleEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    double elapsed = state.time - m_start;

    if (elapsed < 0) {
        return;
    }

    if (elapsed > m_duration) {
        m_start = state.time + rand() % 7;
        float x = rand() % buffer.width();
        float y = rand() % buffer.height();
        circle.set_center(Point(x, y));
        return;
    }

    float progress = (m_duration / 2.0f) - std::abs(elapsed - (m_duration / 2.0f));
    circle.set_radius(progress);
    circle.fill(buffer, state);
}
