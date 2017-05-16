#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include "C:\Users\tobi\Desktop\pa_stable_v190600_20161030\pa_stable_v190600_20161030\portaudio\include\portaudio.h"

class Sound {
public:
    Sound();
    ~Sound();

    int play(char* data);
    void stop(int streamID);

private:
    std::map<int, PaStream*> m_streams;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
