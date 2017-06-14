#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <vector>
#include <memory>


typedef std::vector<unsigned int>  EffectBuffer;

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
    const RotationData& rotation;

    EffectState(const double time, const RotationData& rotation)
    : time(time), rotation(rotation)
    {}
};

struct Point
{
    unsigned int x;
    unsigned int y;

    Point(unsigned int x, unsigned int y)
    : x(x), y(y)
    {}
};


class Effect {
public:
    virtual ~Effect() {}
    virtual void fill(EffectBuffer& buffer, const EffectState& state) = 0;
};


class AddEffect : public Effect
{
    std::shared_ptr<Effect> m_base;
    float m_coeff;

public:
    AddEffect(std::shared_ptr<Effect> base, const float coeff);
    void fill(EffectBuffer& buffer, const EffectState& state);

    ~AddEffect();
};

#endif
