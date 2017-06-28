#ifndef SOUND_HPP
#define SOUND_HPP

#include <map>
#include <vector>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <memory>
#include <mutex>
#include <atomic>
#include <iostream>

#include "ox_src/ofxBeat.h"

struct BeatData
{
    float kick;
    float snare;
    float hihat;

    BeatData(float kick, float snare, float hihat)
    : kick(kick), snare(snare), hihat(hihat)
    {}

    BeatData() noexcept
    : kick(0), snare(0), hihat(0)
    {}
};

class BeatBuffer {
    std::vector<float> m_data;
    std::atomic<size_t> m_pos;
    std::mutex m_mutex;
    ofxBeat m_beat;

public:
    BeatBuffer();

    void put_frame(const float frame)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_pos > m_data.size()) {
            std::cout << "Beat buffer overrun" << std::endl;
            return;
        }
        m_data[m_pos++] = frame;
    }

    void process(int time);

    float kick() { return m_beat.kick(); }
    float snare() { return m_beat.snare(); }
    float hihat() { return m_beat.hihat(); }
};

class Stream {
public:
    FILE* m_file = nullptr;
    OggVorbis_File m_vorbisfile;
    PaStream* m_stream = nullptr;
    bool m_loop = false;
    double m_time = 0.0;
    float m_volume = 1.0f;
    std::atomic<long> m_seek_seconds;
    BeatBuffer m_beat;
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
    BeatData getBeatData(int streamID, int time);

private:
    std::map<int, std::shared_ptr<Stream>> m_streams;
    std::mutex m_mutex;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
