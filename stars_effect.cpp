
#include "stars_effect.hpp"
#include <algorithm>
#include <cmath>
#include "color_utils.hpp"

int frames = 60;

StarsEffect::StarsEffect()
{
    int numStars = 10;
    for (int i=0; i<numStars; i++)
        m_stars.push_back(Star());
}

float t(float x)
{
    if (x <= frames / 2)
        return std::pow(x, 0.5f) / std::pow(frames / 2, 0.5f);
    if (x > frames / 2)
       return std::pow(frames / 2 - (x - frames / 2), 0.5f) / std::pow(frames / 2, 0.5f);
}

void StarsEffect::fill(EffectBuffer& buffer, const EffectState& state) {

    for (auto& i: m_stars)
        if (rand() % 20 == 0)
            if (!i.m_enabled) {
                i.m_enabled = true;
                i.m_time = 0.0f;
                i.m_x = rand() % buffer.width();
                i.m_y = rand() % buffer.height();
                i.m_color = Color3(0.5, 0.5, 1);
                i.m_color = HSVtoRGB(Color3(rand()/(float)RAND_MAX, 1.0f, 0.5f));
            }

    for (auto& i: m_stars) {
        i.m_time++;
        if (i.m_time > frames)
            i.m_enabled = false;

        if (i.m_enabled)
            buffer.set(i.m_x, i.m_y, i.m_color * t(i.m_time));
    }
}

