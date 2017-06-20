
#include "stars_effect.hpp"
#include <algorithm>
#include <cmath>
#include "color_utils.hpp"

float show_time = 1.7f;

StarsEffect::StarsEffect(std::shared_ptr<ColorMaker> color_maker, const Rect& box, int num_stars)
: m_color_maker(color_maker), m_box(box)
{
    for (int i=0; i<num_stars; i++)
        m_stars.push_back(Star());
}

float fac = 0.5;

float t(float x)
{
    if (x <= show_time / 2.0f)
        return std::pow(x, fac) / std::pow(show_time / 2, fac);
    if (x > show_time / 2.0f)
       return std::pow(show_time / 2 - (x - show_time / 2), fac) / std::pow(show_time / 2, fac);
}

void StarsEffect::fill(EffectBuffer& buffer, const EffectState& state) {
    int started = 0;
    for (auto& star: m_stars) {
        float elapsed = state.time - star.m_time;

        if (elapsed < 0) {
            continue;
        }

        if (elapsed > show_time) {
            if (started > 2) {
                continue;
            }
            star.m_time = state.time + (rand() % 800) / 100.0f;
            star.m_x = (rand() % int(m_box.width())) + m_box.top_left.x;
            star.m_y = (rand() % int(m_box.height())) + m_box.top_left.y;
            star.m_color = m_color_maker->make(state);
            started ++;
            continue;
        }

        buffer.set(star.m_x, star.m_y, star.m_color * t(elapsed));
    }
}

