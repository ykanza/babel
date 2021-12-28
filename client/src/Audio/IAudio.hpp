//
// Created by yheb on 06/10/2021.
//

#ifndef BABEL_IAUDIO_HPP
#define BABEL_IAUDIO_HPP

class IAudio {
public:
    virtual void store(std::vector<float> receivedSamplesPack) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif //BABEL_IAUDIO_HPP
