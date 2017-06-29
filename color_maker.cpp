#include "color_maker.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>

#include "color_utils.hpp"

Color3 RandomColorMaker::make(const EffectState& state)
{
    return HSVtoRGB(Color3(rand()/(float)RAND_MAX, 1.0f, 0.5f));
}

#define KICK_GLOW 2.0f

Color3 KickColorIntensity::make(const EffectState& state)
{
    float current = state.sound.kick;


    if (current > 0.2f) {
        m_onset_time = state.time;
    }

    float elapsed = state.time - m_onset_time;
    float coeff = 0;
    if (elapsed < KICK_GLOW) {
        float progress = elapsed / KICK_GLOW;
	coeff = m_last * (1.0f - std::pow(progress, 0.4f));
    }

    if (current > 0 && coeff < current) {
	m_last = current;
    }

    return m_color * std::max(current, coeff);
}
