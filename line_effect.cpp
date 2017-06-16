#include <cmath>
#include "line_effect.hpp"
#include "color_utils.hpp"


LineEffect::LineEffect(const Point& p1, const Point& p2, const Color3 color)
: m_p1(p1), m_p2(p2), m_color(color)
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
            buffer.set(x, y, m_color * fabs(m_rot - state.rotation.x) * 10.0f);
        }
    } else {
        float y = m_p1.y;
        for (int x=m_p1.x; x<=m_p2.x; x++) {
            y += dy;
            buffer.set(x, y, m_color * fabs(m_rot - state.rotation.x) * 10.0f);
        }
    }
    m_rot = state.rotation.x;
}
