#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <memory>

class Stream {
public:
    FILE* m_file = nullptr;
    std::shared_ptr<OggVorbis_File> m_vorbisfile;
    PaStream* m_stream = nullptr;
};

class Sound {
public:
    Sound();
    virtual ~Sound();

    int play(const std::string& filename);
    void stop(int streamID);

private:
    std::map<int, Stream> m_streams;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
