#ifndef INETWORKCLIENT_H
#define INETWORKCLIENT_H

#include "Protocol/Protocol.hpp"

class InetworkClient
{
public:
    virtual ~InetworkClient() = default;;

    virtual void connectToServer(std::string ip, const int &port) = 0;
    virtual void readPendingDatagrams() = 0;
    virtual void writeDatagrams(EncodedPacket encodedPacket) = 0;
    
    /* Call method */
    virtual void callOtherUser(int id) = 0;
    virtual void getCallFromOtherClient(const Body &body) = 0;
    virtual void handleCallAcceptation(const Body &body) = 0;
    virtual void launchCall() = 0;
    /***************/

    virtual void askForUserList() = 0;
    virtual const std::vector<int> &getUserList() = 0;
    virtual void setUserList(const Body &body) = 0;
    
    virtual void hangUp() = 0;
    virtual void setHangUp(const Body &body) = 0;

    virtual void handlePackage(const Package &package) = 0;
    virtual void listen() = 0;
    virtual void sendAudio(const EncodedPacket& encodedPacket) = 0;
    virtual void handleAudio(const Body &body) = 0;
    
    virtual void connected() = 0;
    virtual void disconnected() = 0;
    virtual void readyRead() = 0;
};


#endif