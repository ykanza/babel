/*
** EPITECH PROJECT, 2021
** babel
** File description:
** TODO: add description
*/
#ifndef BABEL_OPUS_HPP
#define BABEL_OPUS_HPP

#include <vector>
#include "ICodec.hpp"

class Opus : ICodec {
    public:
        Opus();
        ~Opus();

    public:
        EncodedPacket encode(const std::vector<float> &samplesPack) override;
        std::vector<float> decode(const EncodedPacket& encodedPacket) override;

    private:
        OpusDecoder *decoder;
        OpusEncoder *encoder;
};

#endif //BABEL_OPUS_HPP
