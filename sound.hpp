#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <memory>
#include <mutex>
#include <atomic>

class Stream {
public:
    FILE* m_file = nullptr;
    OggVorbis_File m_vorbisfile;
    PaStream* m_stream = nullptr;
    bool m_loop = false;
    double m_time = 0.0;
    float m_volume = 1.0f;
    std::atomic<long> m_seek_seconds;
};

class Sound {
public:
    Sound();
    virtual ~Sound();

    int play(const std::string& filename, bool loop = false, float volume = 1.0f);
    void stop(int streamID);
    double getTime(int streamID);
    void setVolume(int streamID, float volume);
    float getVolume(int streamID);
    void seekSeconds(int streamID, long seek_seconds);

private:
    std::map<int, std::shared_ptr<Stream>> m_streams;
    std::mutex m_mutex;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
