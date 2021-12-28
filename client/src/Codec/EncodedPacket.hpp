//
// Created by yheb on 09/10/2021.
//

#ifndef BABEL_ENCODEDPACKET_HPP
#define BABEL_ENCODEDPACKET_HPP

#include <Config/Config.hpp>

struct EncodedPacket {
    char data[Config::FRAMES_PER_BUFFER * Config::CHANNELS * sizeof(float)];
    int bytes;
};

#endif //BABEL_ENCODEDPACKET_HPP
