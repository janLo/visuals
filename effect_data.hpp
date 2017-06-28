#ifndef EFFECT_DATA_HPP
#define EFFECT_DATA_HPP

struct BeatEffectData
{
    float kick;
    float snare;
    float hihat;

    BeatEffectData(float kick, float snare, float hihat) noexcept
    : kick(kick), snare(snare), hihat(hihat)
    {}

    BeatEffectData() noexcept
    : kick(0), snare(0), hihat(0)
    {}
};

struct RotationData
{
    float x, y, z; // angles
    RotationData(const float x, const float y, const float z)
    : x(x), y(y), z(z)
    {}
};

struct EffectState
{
    const double time;
    const RotationData rotation;
    const BeatEffectData sound;

    EffectState(const double time, const RotationData& rotation, const BeatEffectData& sound)
    : time(time), rotation(rotation), sound(sound)
    {}
};

#endif
