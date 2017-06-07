#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <memory>

class Stream {
public:
    FILE* m_file = nullptr;
    OggVorbis_File m_vorbisfile;
    PaStream* m_stream = nullptr;
    bool m_loop = false;
    double m_time = 0.0;
    float m_volume = 1.0f;
};

class Sound {
public:
    Sound();
    virtual ~Sound();

    int play(const std::string& filename, bool loop = false);
    void stop(int streamID);
    double getTime(int streamID);
    void setVolume(int streamID, float volume);
    float getVolume(int streamID);

private:
    std::map<int, std::shared_ptr<Stream>> m_streams;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
