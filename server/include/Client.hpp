//
// Created by yheb on 14/10/2021.
//

#ifndef BABEL_CLIENT_HPP
#define BABEL_CLIENT_HPP

struct Client {
    Client(int _id, asio::ip::tcp::socket _socket) : id(_id), isConnected(true), socket(std::move(_socket)), nbBytesRead(0), package({DEFAULT}) {}
    int id;
    bool isConnected;
    asio::ip::tcp::socket socket;
    int nbBytesRead;
    Package package;
};

#endif //BABEL_CLIENT_HPP
