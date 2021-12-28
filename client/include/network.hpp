#ifndef NETWORK_CLI_H
#define NETWORK_CLI_H

#include "QObject"
#include "QTcpSocket"
#include "QtNetwork"
#include "QString"
// #include "QDebug7gt"
#include <string>
#include <iostream>
//a retirer, pour le sleep
#include <unistd.h>
#include <server/src/Exchange/Exchange.hpp>
#include "Protocol/Protocol.hpp"
#include "Inetwork.hpp"
#include "../src/Audio/PA.hpp"

class networkCli : public QObject, public InetworkClient
{
    Q_OBJECT
private:
    /* A CHANGER EN SMART POINTER */
    QTcpSocket *socket;
    QUdpSocket *UDPSocket;
    bool inCall;
    Exchange exchange;
    std::thread threadCall;
    std::function<void(EncodedPacket packet)> handleReceivedSamplesPack;
    std::function<void()> stopAudio;
    std::vector<int> userList;

public:
    explicit networkCli(QObject *parent = nullptr);
    ~networkCli() override;
    void connectToServer(std::string ip, const int &port) override;
    void writeDatagrams(EncodedPacket encodedPacket) override;
    
    /* Call method */
    void callOtherUser(int id) override;
    void getCallFromOtherClient(const Body &body) override;
    void handleCallAcceptation(const Body &body) override;
    void launchCall() override;
    /***************/

    void askForUserList() override;
    const std::vector<int> &getUserList() override;


    void setHangUp(const Body &body) override;



    void handlePackage(const Package &package) override;
    void listen() override;
    void sendAudio(const EncodedPacket& encodedPacket) override; /* set audio var*/
    void handleAudio(const Body &body) override;

signals:
    void signalUserList();
    void signalHangUp();

public:
    void hangUp() override;

public slots:
    void setUserList(const Body &body) override;

private slots:
    void connected() override;
    void disconnected() override;
    void readyRead() override;
    void readPendingDatagrams() override;
};

#endif