#include <cmath>
#include "line_effect.hpp"
#include "color_utils.hpp"


LineEffect::LineEffect(const Point& p1, const Point& p2, const Color3 color, unsigned int width)
: m_p1(p1), m_p2(p2), m_color(color), m_width(width)
{}


void LineEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    float dx = float(m_p2.x-m_p1.x)/float(m_p2.y-m_p1.y);
    float dy = float(m_p2.y-m_p1.y)/float(m_p2.x-m_p1.x);
    float len = length(m_p2.x-m_p1.x, m_p2.y-m_p1.y);

    if (dx < dy) {
        float x = m_p1.x;
        for (int y=m_p1.y; y<=m_p2.y; y++) {
            x += dx;
            buffer[y * m_width + x] = m_color;
        }
    } else {
        float y = m_p1.y;
        for (int x=m_p1.x; x<=m_p2.x; x++) {
            y += dy;
            buffer[(int)y * m_width + x] = m_color;
        }
    }
}
