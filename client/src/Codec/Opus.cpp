/*
** EPITECH PROJECT, 2021
** babel
** File description:
** TODO: add description
*/

#include <vector>
#include <Config/Config.hpp>
#include "Opus.hpp"
#include "Exception/Error.hpp"

Opus::Opus() {
    int err = 0;
    std::string bytesErr;

    this->decoder = opus_decoder_create(Config::SAMPLE_RATE, Config::CHANNELS, &err);
    if (err != OPUS_OK) {
        bytesErr = opus_strerror(err);
        throw CodecError(bytesErr, "Opus");
    }
    this->encoder = opus_encoder_create(Config::SAMPLE_RATE, Config::CHANNELS, Config::APPLICATION, &err);
    if (err != OPUS_OK) {
        bytesErr = opus_strerror(err);
        throw CodecError(bytesErr, "Opus");
    }
}

EncodedPacket Opus::encode(const std::vector<float>& samplesPack) {
    int res = 0;
    EncodedPacket encodedPacket = {0};
    int encodedPacketSize = Config::FRAMES_PER_BUFFER * Config::CHANNELS * sizeof(float);

    res = opus_encoder_ctl(this->encoder, OPUS_SET_BITRATE(OPUS_AUTO));
    if (res < 0) {
        std::string bytesErr(opus_strerror(res));
        throw CodecError(bytesErr, "Opus");
    }

    encodedPacket.bytes = opus_encode_float(this->encoder, samplesPack.data(),
    Config::FRAMES_PER_BUFFER, reinterpret_cast<unsigned char *>(encodedPacket.data), encodedPacketSize);
    if (encodedPacket.bytes < 0) {
        std::string bytesErr(opus_strerror(encodedPacket.bytes));
        throw CodecError(bytesErr, "Opus");
    }

    return encodedPacket;
}

std::vector<float> Opus::decode(const EncodedPacket& encodedPacket) {
    int bytes = 0;
    std::vector<float> decodedPacket(Config::FRAMES_PER_BUFFER * Config::CHANNELS);

    bytes = opus_decode_float(this->decoder, reinterpret_cast<unsigned const char *>(encodedPacket.data),
    encodedPacket.bytes, decodedPacket.data(), Config::FRAMES_PER_BUFFER, 0);
    if (bytes < 0) {
        std::string bytesErr(opus_strerror(bytes));
        throw CodecError(bytesErr, "Opus");
    }

    return decodedPacket;
}

Opus::~Opus() {
    opus_encoder_destroy(this->encoder);
    opus_decoder_destroy(this->decoder);
}