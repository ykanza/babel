//
// Created by yheb on 17/10/2021.
//

#ifndef BABEL_CONFIG_HPP
#define BABEL_CONFIG_HPP

#include <opus.h>

class Config {
public:
    static const int FRAMES_PER_BUFFER = 2880;
    static const int SAMPLE_RATE = 48000;
    static const int CHANNELS = 1;
    static const int APPLICATION = OPUS_APPLICATION_VOIP;
    static const bool UDP = true;
};

#endif //BABEL_CONFIG_HPP
