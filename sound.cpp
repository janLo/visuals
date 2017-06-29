#define NOMINMAX
#include "sound.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <memory>

Sound::Sound()
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }
}

Sound::~Sound()
{
    PaError err = Pa_Terminate();
    if (err != paNoError)
        std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;

}

int Sound::play(const std::string& filename, bool loop, float volume)
{
    auto s = std::make_shared<Stream>();
    s->m_loop = loop;
    s->m_file = fopen(filename.c_str(), "rb");
    if (!s->m_file) {
        std::cout << "Sound::play error: file not found: " << filename << std::endl;
        return 0;
    }
    if (ov_open(s->m_file, &s->m_vorbisfile, nullptr, 0) < 0) {
          std::cout << "Sound::play error: input does not appear to be an Ogg bitstream." << std::endl;
          return 0;
    }

    vorbis_info* vorbisInfo = ov_info(&s->m_vorbisfile, -1);

    PaStream* stream = nullptr;
    PaError err = Pa_OpenDefaultStream(&stream,
        0,                              // no input channels
        vorbisInfo->channels,           // stereo output
        paFloat32,                      // 32 bit floating point output
        vorbisInfo->rate,
        paFramesPerBufferUnspecified,   // frames per buffer, i.e. the number
                                        //  of sample frames that PortAudio will
                                        //  request from the callback. Many apps
                                        //  may want to use
                                        //  paFramesPerBufferUnspecified, which
                                        //  tells PortAudio to pick the best,
                                        //  possibly changing, buffer size.
        &Sound::callback,               // this is your callback function
        s.get());                       // This is a pointer that will be passed to
                                        //  your callback
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }
    s->m_stream = stream;
    volume = std::max(0.0f, std::min(1.0f, volume));
    s->m_volume = volume;

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_streams[m_streamIDNext++] = s;
    
    return m_streamIDNext - 1;
}

void Sound::stop(int streamID)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return;

    PaError err = Pa_StopStream(stream->second->m_stream);
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }

    //ov_clear(&stream->second->m_vorbisfile);
    fclose(stream->second->m_file);
    m_streams.erase(stream);
}

double Sound::getTime(int streamID)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return 0.0;

    //return Pa_GetStreamTime(stream->second->m_stream); // doesn't work for some reason
    return stream->second->m_time;
}

void Sound::setVolume(int streamID, float volume)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    volume = std::max(0.0f, std::min(1.0f, volume));

    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return;

    stream->second->m_volume = volume;
}

float Sound::getVolume(int streamID)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return 0.0f;

    return stream->second->m_volume;
}

void Sound::seekSeconds(int streamID, long seek_seconds)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return;

    stream->second->m_seek_seconds += seek_seconds;
}

int Sound::callback(const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    Stream* stream = static_cast<Stream*>(userData);
    float* out = static_cast<float*>(outputBuffer);

    long seek_seconds = stream->m_seek_seconds.exchange(0);
    if (0 != seek_seconds) {
        double current_pos = ov_time_tell(&stream->m_vorbisfile);
        double length = ov_time_total(&stream->m_vorbisfile, -1);

        current_pos = std::max(0.0, std::min(length, current_pos + double(seek_seconds)));
        ov_time_seek_lap(&stream->m_vorbisfile, current_pos);
    }

    unsigned long samples = 0;
    while(samples < framesPerBuffer) {
        float** vorbisOut = nullptr;
        int current_section = 0;
        long ret = ov_read_float(&stream->m_vorbisfile, &vorbisOut, framesPerBuffer - samples, &current_section);
        if (!ret) {
            if (stream->m_loop)
                ov_raw_seek(&stream->m_vorbisfile, 0);
            break; // EOF
        } else if (ret < 0) {
            if (ret == OV_EBADLINK) {
                std::cout << "Sound::callback error: Corrupt bitstream section!" << std::endl;
                break;
            }
        }

        if (ret > 0 && vorbisOut != nullptr) {
            stream->m_beat.put_frames(vorbisOut[0], ret);
        }

        for(int i=0; i<ret; i++)
        {
            *out++ = vorbisOut[0][i] * stream->m_volume;
            *out++ = vorbisOut[1][i] * stream->m_volume;
        }

        samples += ret;
    }

    stream->m_time = timeInfo->outputBufferDacTime;

    return paNoError;
}


BeatEffectData Sound::getBeatData(int streamID, int time) {
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return BeatEffectData(0, 0, 0);

    BeatBuffer& bb = stream->second->m_beat;
    bb.process(time);
    return BeatEffectData(bb.kick(), bb.snare(), bb.hihat());
}


BeatBuffer::BeatBuffer()
: m_data(1024 * 2), m_beat(1024 * 2, 512)
{ }


void BeatBuffer::process(int time)
{
    size_t pos(0);
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        pos = m_pos.exchange(0);
        if (0 == pos) {
            return;
        }
        m_beat.audioFill(m_data.data(), pos);
    }
    m_beat.audioProcess(pos);
    m_beat.update(time);
}
