#ifndef INETWORK_H
#define INETWORK_H

#include "../include/protocolManager.hpp"
#include "Client.hpp"

class InetworkServer
{
public:
    virtual ~InetworkServer() = default;;

        /*  Async method */
    virtual void getNewClientConnections() = 0;
    virtual void setSocketRead(std::shared_ptr<Client> client) = 0;
    /*****************/

    virtual void openConnections() = 0;
    virtual void closeConnections() = 0;

    /*  Package response method */
    virtual void callResponse(std::shared_ptr<Client> client) = 0;
    virtual void acceptCallResponse(std::shared_ptr<Client> client) = 0;
    virtual void receiveAudio(std::shared_ptr<Client> client) = 0;
    virtual void userListResponse(std::shared_ptr<Client> client) = 0;
    virtual void userHangUpResponse(std::shared_ptr<Client> client) = 0;
    virtual void handlePackage(std::shared_ptr<Client> client) = 0;
    /****************************/

};

#endif