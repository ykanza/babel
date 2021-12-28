//
// Created by yheb on 06/10/2021.
//

#ifndef BABEL_PA_HPP
#define BABEL_PA_HPP


#include <portaudio.h>
#include <vector>
#include <client/src/Codec/Opus.hpp>
#include <functional>
#include "IAudio.hpp"

enum StreamType {
    INPUT,
    OUTPUT
};

class PA : public IAudio {
public:
    explicit PA(const std::function<void(const std::vector<float>&)>& _handleSamplesPack);
    void store(std::vector<float> receivedSamplesPack) override;
    void start() override;
    void stop() override;
    ~PA();
private:
    std::function<void(std::vector<float>)> handleSamplesPack;
    PaStream *inputStream;
    PaStream *outputStream;
    std::vector<std::vector<float>> recordedSamplesPacks;
    std::vector<std::vector<float>> receivedSamplesPacks;
    static int playCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
    static int recordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
    void openStream(StreamType type);
    void openStreams();
    void startStreams();
    void closeStreams();
};

#endif //BABEL_PA_HPP
