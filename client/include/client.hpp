#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <asio.hpp>


class client
{
private:
    /* data */
public:
    client(std::string ip, int port);
    ~client();
};

#endif