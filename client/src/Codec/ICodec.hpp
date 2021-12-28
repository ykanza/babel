/*
** EPITECH PROJECT, 2021
** babel
** File description:
** TODO: add description
*/
#ifndef BABEL_ICODEC_HPP
#define BABEL_ICODEC_HPP

#include <opus.h>
#include <iostream>
#include <thread>
#include <stdexcept>
#include "EncodedPacket.hpp"

class ICodec {
    public:
        virtual EncodedPacket encode(const std::vector<float>& samplesPack) = 0;
        virtual std::vector<float> decode(const EncodedPacket& encodedPacket) = 0;
};


#endif //BABEL_ICODEC_HPP
