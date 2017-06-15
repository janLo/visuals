#include "filled_circle_effect.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>


CircleEffect::CircleEffect(const& Point center, const float radius, const Color3& color, const bool fill, const double time)
: m_center(center), m_radius(radius), m_color(color), m_fill(fill)
{}

void CircleEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    const int x_min = std::floor(m_center.y - m_radius) - 1;
    const int x_max = std::ceil(m_center.y + m_radius) + 1;

    const int y_min = std::floor(m_center.x - m_radius) - 1;
    const int y_max = std::ceil(m_center.x + m_radius) + 1;

    std::cout << x_min << " " << x_max << " " <<  y_min << " " << y_max << std::endl;

    const float rad_square = m_radius * m_radius;

    for (int y = y_min; y < y_max; ++y) {
        float b = float(y) - m_center.y;
        for (int x = x_min; x < x_max; ++x) {
            float a = float(x) - m_center.x;
            float d = std::sqrt(a*a + b*b) - m_radius;

            if (m_fill) {
                if (d < 1.0 && d > -1.0) {
                    float coeff = float(std::min(1.0f, 1.0f - d));
                    buffer.set(x % buffer.width(), y % buffer.height(), m_color * coeff);
                }
            } else {
                if (d < 1.0 && d > -1.0) {
                    float coeff = (d > 0 ? 1.0f - d : 1.0f + d);
                    buffer.set(x % buffer.width(), y % buffer.height(), m_color * float(std::min(1.0f, coeff)));
                }
            }
        }
    }
}
