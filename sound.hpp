#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <memory>

class Channel {
public:
    FILE* m_file;
    std::shared_ptr<OggVorbis_File> m_vorbisfile;
    PaStream* m_stream;
};

class Sound {
public:
    Sound();
    ~Sound();

    int play(const std::string& filename);
    void stop(int streamID);

private:
    std::map<int, PaStream*> m_streams;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
