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

    float factor = std::fabs(m_rot - state.rotation.x) * 5.0f;
    factor = factor * 0.8f + m_lastFactor * 0.2f;
    if (factor > 20)
        factor = m_lastFactor;
    if (factor < m_limitHigh)
        m_limitState = false;
    if (factor > m_limitLow)
        m_limitState = true;
    if (!m_limitState)
        factor = 0.0f;
    if (dx < dy) {
        float x = m_p1.x;
        for (int y=m_p1.y; y<=m_p2.y; y++) {
            x += dx;
            buffer.set(x, y, m_color * factor);
        }
    } else {
        float y = m_p1.y;
        for (int x=m_p1.x; x<=m_p2.x; x++) {
            y += dy;
            buffer.set(x, y, m_color * factor);
        }
    }
    m_rot = state.rotation.x;
    m_lastFactor = factor;
}
