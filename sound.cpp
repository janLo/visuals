#include "sound.hpp"
#include <sstream>
#include <iostream>

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

int Sound::play(char* data)
{
    PaStream* stream = nullptr;
    PaError err = Pa_OpenDefaultStream(&stream,
        0,              // no input channels
        2,              // stereo output
        paFloat32,      // 32 bit floating point output
        48000,
        paFramesPerBufferUnspecified,   // frames per buffer, i.e. the number
                                        //  of sample frames that PortAudio will
                                        //  request from the callback. Many apps
                                        //  may want to use
                                        //  paFramesPerBufferUnspecified, which
                                        //  tells PortAudio to pick the best,
                                        //  possibly changing, buffer size.
        &Sound::callback,               // this is your callback function
        this);                          // This is a pointer that will be passed to
                                        //  your callback
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }

    m_streams[m_streamIDNext++] = stream;
    
    return m_streamIDNext - 1;
}

void Sound::stop(int streamID)
{
    auto stream = m_streams.find(streamID);
    if (stream == m_streams.end())
        return;

    PaError err = Pa_StopStream(stream->second);
    if (err != paNoError) {
        std::stringstream ss;
        ss << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error(ss.str());
    }
}

int Sound::callback(const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    // Cast data passed through stream to our structure.
    Sound* sound = static_cast<Sound*>(userData);
    float* out = static_cast<float*>(outputBuffer);
    unsigned int i;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = i/256.0f;
        *out++ = i/256.0f;
/*        *out++ = data->left_phase;  // left
        *out++ = data->right_phase;  // right*/
    }
    return 0;
}