#include <client/src/Audio/PA.hpp>
#include <Utils/Utils.hpp>
#include <cstring>
#include "../include/network.hpp"
#include "Exception/Error.hpp"


networkCli::networkCli(QObject *parent) : QObject(parent)
{
    /* Create Client socket to communicate with server */
    this->socket = new QTcpSocket(this);
    this->UDPSocket = new QUdpSocket(this);
    connect(this->UDPSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    qDebug() << "[OK] - A new socket has been created!";
    inCall = false;
    this->exchange = {0};
}

void networkCli::writeDatagrams(EncodedPacket encodedPacket)
{
    QByteArray Data;

    Data.append(encodedPacket.data, encodedPacket.bytes);
    this->UDPSocket->writeDatagram(Data, QHostAddress(this->exchange.IP.c_str()), this->exchange.port);
}

void networkCli::readPendingDatagrams()
{
    qDebug() << "in !";
    // when data comes in
    if (!this->UDPSocket->hasPendingDatagrams())
        return;
    QByteArray buffer;
    buffer.resize(this->UDPSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    EncodedPacket encodedPacket = {0};

    this->UDPSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    std::strcpy(encodedPacket.data, buffer.data());
    encodedPacket.bytes = buffer.size();

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer.data();
    qDebug() << "SIZE = " << buffer.size();

    if (this->handleReceivedSamplesPack)
        this->handleReceivedSamplesPack(encodedPacket);
}

networkCli::~networkCli()
{
    if (this->threadCall.joinable())
        this->threadCall.join();
    socket->close();
}

void networkCli::connected()
{ /* Upon connected signals, it is redirected here */
    qDebug() << "[NOTIFICATIONS] - You are connected to server.";
}

void networkCli::disconnected()
{ /* Upon disconnected signals, it is redirected here */
    qDebug() << "[NOTIFICATIONS] - You have been disconected from the server.";
}

/*------------------    Call related -----------------*/
void networkCli::callOtherUser(int id)
{   /* Call another client */
    /* Create request */
    userReqCall subRequest { id, 0, 0};
    userReq request = {0};
    request.call = subRequest;
    /* Assemble structure */
    Package package = {DEFAULT};
    package.body.pkg.req = request;
    package.header = {TYPE::CALL_REQ};

    std::cout << "Size of Package " << sizeof(Package) << std::endl;
    if (socket->waitForConnected(3000)) {
        socket->write(reinterpret_cast<char*>(&package), sizeof(Package));
        socket->waitForBytesWritten();
    }
}

void networkCli::getCallFromOtherClient(const Body &body)
{  /* Upon getting Call PKG, should answers y or n to send another pkg to first client */
    char answer = 0;

    std::cout << "You received a call from ";
    std::cout << body.pkg.res.call.infoCaller.ip << ":" << body.pkg.res.call.infoCaller.port << std::endl;
    std::cout << "user ID: " << body.pkg.res.call.infoCaller.id << std::endl;
    std::cout << "[y/n]" << std::endl;
    std::cin >> answer;

        /* Create request */
    this->exchange.id = body.pkg.res.call.infoCaller.id;
    this->exchange = {
            body.pkg.res.call.infoCaller.id,
            body.pkg.res.call.infoCaller.ip,
            body.pkg.res.call.infoCaller.port
    };
    Package package = {DEFAULT};
    userReqCall subRequest = {0};
    subRequest.idCalled = body.pkg.res.call.infoCalled.id;
    subRequest.idCaller = body.pkg.res.call.infoCaller.id;
    subRequest.whoCalled = 1;
    subRequest.accept = (answer == 'y');
    userReq request = {0};
    request.call = subRequest;
    /* Assemble structure */
    package.body.pkg.req = request;
    package.header = {TYPE::CALL_REQ};

    std::cout << "Size of Package " << sizeof(Package) << std::endl;
    if (socket->waitForConnected(3000)) {
        socket->write(reinterpret_cast<char*>(&package), sizeof(Package));
        socket->waitForBytesWritten();
        if (subRequest.accept) {
            this->inCall = true;
            if (Config::UDP)
                this->UDPSocket->bind(QHostAddress(body.pkg.res.call.infoCalled.ip), body.pkg.res.call.infoCalled.port);
            this->launchCall();
        }
    }
}

void networkCli::handleCallAcceptation(const Body &body)
{
    if (body.pkg.res.call.accept == true) {
        this->exchange.id = body.pkg.res.call.infoCalled.id;
        this->exchange = {body.pkg.res.call.infoCalled.id, body.pkg.res.call.infoCalled.ip, body.pkg.res.call.infoCalled.port};
        if (Config::UDP)
            this->UDPSocket->bind(QHostAddress(body.pkg.res.call.infoCaller.ip), body.pkg.res.call.infoCaller.port);
        this->inCall = true;
        std::cout << "User has accepted your call." << std::endl;
        this->launchCall();
    } else {
        std::cout << "User has declined your call." << std::endl;
    }
}

void networkCli::launchCall() {
    this->threadCall = std::thread([this]() {
        //TODO: getInstance('Class name');
        Opus codec;

        PA audio = PA([&codec, this](const std::vector<float>& recordedSamplesPack) {
            EncodedPacket encodedPacket = codec.encode(recordedSamplesPack);
            std::cout << "exchange id = " << this->exchange.id << "-> " << this->exchange.IP <<":" << this->exchange.port << std::endl;
            if (Config::UDP)
                this->writeDatagrams(encodedPacket);
            else
                this->sendAudio(encodedPacket);
        });
        this->handleReceivedSamplesPack = ([&codec, &audio, this] (EncodedPacket encodedPacket) {
            std::vector<float> receivedSamplesPack = codec.decode(encodedPacket);
            audio.store(receivedSamplesPack);
        });
        this->stopAudio = ([&audio, this] () {
            std::cout << "HANG UP !" << std::endl;
            this->handleReceivedSamplesPack = nullptr;
            audio.stop();
            if (this->threadCall.joinable())
                this->threadCall.join();
        });
        audio.start();
    });
}
/*-----------------------------------------------------*/

/*-------------------  Audio related ------------------*/
void networkCli::sendAudio(const EncodedPacket& encodedPacket)    /* Try to connect to other client so it is C2C instead of C2S2C */
{
     if (!this->inCall)
         return;
    /* Create request */
    Package package = {DEFAULT};
    package.header = {TYPE::AUDIO_REQ};
    package.body.pkg.req.audio = { this->exchange.id, encodedPacket };

    std::cout << "Size of Header package " << sizeof(Header) << std::endl;
    std::cout << "Size of Body package " << sizeof(Body) << std::endl;
    if (socket->waitForConnected()) {
        socket->write(reinterpret_cast<char*>(&package), sizeof(Package));
        socket->waitForBytesWritten();
    }
}

void networkCli::handleAudio(const Body &body)
{
    if (this->handleReceivedSamplesPack) {
        std::cout << "Pkg from ID: " << body.pkg.res.audio.fromID << std::endl;
        std::cout << "encodedPacket bytes" << body.pkg.res.audio.encodedPacket.bytes << std::endl;
        this->handleReceivedSamplesPack(body.pkg.res.audio.encodedPacket);
    }
}

/*-----------------------------------------------------*/

void networkCli::askForUserList()
{
    /* Assemble structure */
    Package package = {DEFAULT};
    package.header = {TYPE::LIST_REQ};

    std::cout << "Size of Package " << sizeof(Package) << std::endl;
    if (socket->waitForConnected(3000)) {
        socket->write(reinterpret_cast<char*>(&package), sizeof(Package));
        socket->waitForBytesWritten();
    }
}

const std::vector<int> &networkCli::getUserList()
{
    return this->userList;
}

void networkCli::setUserList(const Body &body)
{
    this->userList.clear();
    for (int index = 0; index < body.pkg.res.list.nbUser; index += 1)
        this->userList.push_back(body.pkg.res.list.userList[index]);
    emit signalUserList();
}

void networkCli::hangUp()
{
    Package package = {DEFAULT};
    package.header = {TYPE::HANG_REQ};
    package.body.pkg.req.hang = { this->exchange.id };

    std::cout << "Size of Package " << sizeof(Package) << std::endl;
    if (socket->waitForConnected(3000)) {
        socket->write(reinterpret_cast<char*>(&package), sizeof(Package));
        socket->waitForBytesWritten();
    }
    this->inCall = false;
    if (this->stopAudio)
        this->stopAudio();
}

void networkCli::setHangUp(const Body &body)
{
    this->inCall = false;
    if (this->stopAudio)
        this->stopAudio();
}

void networkCli::handlePackage(const Package &package)
{
    static std::unordered_map<TYPE, std::function<void(const Body &body)>> torchBearer {
        {AUDIO_RES, [this](const Body &body) {
            //TODO: desc to move
            std::cout << "Get AUDIO response from server" << std::endl;
            this->handleAudio(body);
        }},
        {CALL_RES, [this](const Body &body) {
            body.pkg.res.call.whoCalled == 0 ?
            this->getCallFromOtherClient(body) :
            this->handleCallAcceptation(body);
        }},
        {LIST_RES, [this](const Body &body) {
            this->setUserList(body);
        }},
        {HANG_RES, [this](const Body &body) {
            this->setHangUp(body);
        }},
    };

    if (torchBearer.find(package.header.type) == torchBearer.end())
        throw TorchBearerError("Unknown res package type -> " + std::to_string(package.header.type), "Client");
    torchBearer[package.header.type](package.body);
}

void networkCli::readyRead()
{ /* Upon receiving packages data signals, it is redirected here */
    qDebug() << "[NOTIFICATIONS] - You have received a package.";
    unsigned int bytesAvailable = this->socket->bytesAvailable();

    if (bytesAvailable < sizeof(Package))
        return;
    Package package = {DEFAULT};
    auto nbBytesRead = this->socket->read(reinterpret_cast<char *>(&package), sizeof(Package));

    qDebug() << "START-----------------------------";
    qDebug() << "HEADER PACKAGE";
    qDebug() << package.header.type;
    qDebug() << "        --------------";
    qDebug() << "BODY PACKAGE";

    try {
        handlePackage(package);
    } catch(std::exception const &e) {
        std::cout << e.what() << std::endl;
    }
    qDebug() << "END-----------------------------";
}

void networkCli::connectToServer(const std::string ip, const int &port)
{
    /* Redirige les signaux vers des fonctions */
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
 
    /* Connection Client -> Server */
    std::cout << "Trying to connect to " << ip << ":" << port << "..." << std::endl;
    socket->connectToHost(ip.c_str(), port);
    qDebug() << "[OK] - Client connected to server.";

    if(!socket->waitForConnected(3000)) {
        qDebug() << "Not connected!";
        throw ConnectionError("Connection failed", "Client");
    }
}

void networkCli::listen()
{
    this->socket->waitForReadyRead(6000);
}
