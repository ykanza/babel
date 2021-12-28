#include "PA.hpp"
#include "portaudio.h"
#include <cstring>
#include <iostream>
#include <functional>
#include "Exception/Error.hpp"
#include <Config/Config.hpp>

PA::PA(const std::function<void(const std::vector<float>&)>& _handleSamplesPack)
{
    PaError err = paNoError;
    this->inputStream = nullptr;
    this->outputStream = nullptr;
    this->handleSamplesPack = _handleSamplesPack;

    err = Pa_Initialize();
    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");

    PA::openStreams();
}

void PA::openStream(StreamType type)
{
    PaStreamParameters parameters;
    PaError err = paNoError;

    parameters.device = type == INPUT
    ? Pa_GetDefaultInputDevice() : Pa_GetDefaultOutputDevice();
    if (parameters.device == paNoDevice) {
        throw AudioError("Error: No default input device.", "Audio");
    }
    parameters.channelCount = Config::CHANNELS;
    parameters.sampleFormat = paFloat32;
    auto deviceInfo = Pa_GetDeviceInfo(parameters.device);
    if (deviceInfo == nullptr)
        throw AudioError("Devices Not found", "Audio");
    parameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
    parameters.hostApiSpecificStreamInfo = nullptr;

    err = type == INPUT
    ? Pa_OpenStream(&this->inputStream, &parameters, nullptr,
    Config::SAMPLE_RATE, Config::FRAMES_PER_BUFFER, paClipOff, recordCallback, this)
    : Pa_OpenStream(&this->outputStream, nullptr, &parameters,
    Config::SAMPLE_RATE, Config::FRAMES_PER_BUFFER, paClipOff, playCallback, this);

    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");
}

PA::~PA()
{
    this->closeStreams();
    Pa_Terminate();
}

int PA::recordCallback(const void *inputBuffer,
void *outputBuffer, unsigned long framesPerBuffer,
const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
void *userData)
{
    PA *data = (PA *) userData;
    float *rptr = (float *) inputBuffer;
    unsigned long totalFramesPerBuffer = framesPerBuffer * Config::CHANNELS;
    std::vector<float> recordedSamplesPack;

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (inputBuffer == nullptr) {
        for (int i = 0; i < totalFramesPerBuffer; i += 1)
            recordedSamplesPack.push_back(0.0f);
    } else {
        for (int i = 0; i < totalFramesPerBuffer; i += 1)
            recordedSamplesPack.push_back(rptr[i]);
    }

    data->handleSamplesPack(recordedSamplesPack);

    return paContinue;
}

int PA::playCallback(const void *inputBuffer,
void *outputBuffer, unsigned long framesPerBuffer,
const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
void *userData)
{
    PA *data = (PA *) userData;
    float *wptr = (float *) outputBuffer;

    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;
    unsigned long totalFramesPerBuffer = framesPerBuffer * Config::CHANNELS;

    if (data->receivedSamplesPacks.empty()) {
        std::memset(wptr, 0, totalFramesPerBuffer);
    } else {
        std::vector<float> rptr = data->receivedSamplesPacks.back();

        for (int i = 0; i < rptr.size(); i += 1)
            wptr[i] = rptr[i];

        data->receivedSamplesPacks.pop_back();
    }

    return paContinue;
}

void PA::startStreams()
{
    PaError err = paNoError;

    err = Pa_StartStream(this->inputStream);
    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");
    err = Pa_StartStream(this->outputStream);
    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");
}

void PA::closeStreams()
{
    PaError err = paNoError;

    err = Pa_CloseStream(this->inputStream);
    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");

    err = Pa_CloseStream(this->outputStream);
    if (err != paNoError)
        throw AudioError(std::to_string(err), "Audio");
}

void PA::openStreams()
{
    this->openStream(INPUT);
    this->openStream(OUTPUT);
}

void PA::store(std::vector<float> receivedSamplesPack)
{
    this->receivedSamplesPacks.insert(this->receivedSamplesPacks.begin(), receivedSamplesPack);
}

void PA::start()
{
    PaError err = paNoError;

    PA::startStreams();
    while (Pa_IsStreamActive(this->inputStream))
        Pa_Sleep(10);
}

void PA::stop()
{
    if (Pa_IsStreamActive(this->inputStream)) {
        this->recordedSamplesPacks.clear();
        Pa_AbortStream(this->inputStream);
    }

    if (Pa_IsStreamActive(this->outputStream)) {
        this->receivedSamplesPacks.clear();
        Pa_AbortStream(this->outputStream);
    }
}