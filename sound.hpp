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
#include <cstring>

#include "ox_src/ofxBeat.h"
#include "effect_data.hpp"


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
            return;
        }
        m_data[m_pos++] = frame;
    }

    void put_frames(const float * const frames, const size_t length)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        size_t pos = m_pos;
        if (pos + length > m_data.size()) {
            return;
        }
        memcpy(&m_data[pos], frames, sizeof(float) * length);
        m_pos = pos + length;
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
    BeatEffectData getBeatData(int streamID, int time);

private:
    std::map<int, std::shared_ptr<Stream>> m_streams;
    std::mutex m_mutex;
    int m_streamIDNext = 1;
    static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

};

#endif
