#include "plasma_effect.hpp"
#include "color_utils.hpp"

void PlasmaEffect::fill(EffectBuffer& buffer, const EffectState& state)
{
    double time = state.time;
    const float PI = 3.141592f;
    for (int y=0; y<buffer.height(); y++) {
        for (int x=0; x<buffer.width(); x++) {

            float color1 = (sin(dot(x+1.0f, sin(time), y+1.0f, cos(time))*0.6f+time)+1.0f)/2.0f;
            float centerX = buffer.width()/2.0f + buffer.width()/2.0f*sin(-time);

            float centerY = buffer.height()/2.0f + buffer.height()/2.0f*cos(-time);

            float color2 = (cos(length(x - centerX, y - centerY)*0.3f)+1.0f)/2.0f;

            float color = (color1 + color2)/2.0f;

            float red	= (cos(PI*color/0.5f+time)+1.0f)/2.0f;
            float green	= (sin(PI*color/0.5f+time)+1.0f)/2.0f;
            float blue	= (sin(time)+1.0f)/2.0f;

            buffer[y * buffer.width() + x] = Color3(red, green, blue);
        }
    }
}
