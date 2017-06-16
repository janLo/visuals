#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <vector>
#include <memory>
#include "color.hpp"

class EffectBuffer {
public:
    EffectBuffer() : m_buffer(m_width*m_height) { }
    void set(int x, int y, const Color3& color) { m_buffer[y * m_width + x] = color; }
    const Color3& get(int x, int y) const       { return m_buffer[y * m_width + x]; }
    std::vector<Color3>& get()                  { return m_buffer; }
    size_t size() const                         { return m_buffer.size(); }
    int width() const                           { return m_width; }
    int height() const                          { return m_height; }
    void clear()                                { for (auto& i: m_buffer) i = Color3(0); }

    Color3& operator [](size_t i)               { return m_buffer[i]; }
    const Color3& operator [](size_t i) const   { return m_buffer[i]; }

    EffectBuffer& operator =(const EffectBuffer& other) 
    {     
        if (this != &other)
            std::copy(other.m_buffer.begin(), other.m_buffer.end(), m_buffer.begin());
        return *this;
    }
private:
    int m_height = 20;
    int m_width = 25;
    std::vector<Color3> m_buffer;
};

//typedef std::vector<unsigned int>  EffectBuffer;

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

    EffectState(const double time, const RotationData& rotation)
    : time(time), rotation(rotation)
    {}
};

struct Point
{
    float x;
    float y;

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
