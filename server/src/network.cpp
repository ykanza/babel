#include <iostream>
#include <string>
#include <Utils/Utils.hpp>
#include "../include/network.hpp"
#include "Protocol/Protocol.hpp"
#include "../../Exception/Error.hpp"

AsioNetwork::AsioNetwork(uint16_t port) : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

AsioNetwork::~AsioNetwork()
= default;

void AsioNetwork::getNewClientConnections()
{
    std::cout << "Declare async new server connection" << std::endl;
    m_asioAcceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket)
        {
            if (!ec) {
                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << " - id(" << this->clientSockets.size() << ")" << std::endl;
                std::shared_ptr<Client> newClient = std::make_shared<Client>(
                     static_cast<int>(this->clientSockets.size()),
                     std::move(socket)
                );
                this->clientSockets.push_back(std::move(newClient));
                std::cout << "There is actually " << this->clientSockets.size() << " client on the server" << std::endl;
                setSocketRead(this->clientSockets.back());
                this->getNewClientConnections();
            }
        }
    );
}

void AsioNetwork::openConnections()
{
    try {
        getNewClientConnections();
        std::cout << "Server Started" << std::endl;
        m_threadContext = std::thread([this]() { m_asioContext.run(); });
    } catch (ConnectionError const &e) {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
    } catch (TorchBearerError const &e) {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
    }
}

void AsioNetwork::closeConnections()
{
    m_asioContext.stop();
	if (m_threadContext.joinable())
        m_threadContext.join();
    std::cout << "[SERVER] Stopped!\n";
}

/**************************  RESPONSE PACKAGE METHOD ***************************/

void AsioNetwork::callResponse(std::shared_ptr<Client> caller)
{   /* S -> C2 */
    userResCall subResponse = {0};
    userRes response = {0};
    Body bodyResponse = {0};
    Package package = {DEFAULT};
    int i = 0;
    int idCalled = caller->package.body.pkg.req.call.idCalled;
    auto called = this->clientSockets[idCalled];

    std::cout << "[Received] - Body" << std::endl;
    std::cout << "CALL REQUEST" << std::endl;
    std::cout << "[Body pkg] calling id: " << caller->package.body.pkg.req.call.idCalled << std::endl;
    std::cout << "Send package to id: " << idCalled << std::endl;

    subResponse.infoCalled.id = called->id;
    std::strcpy(subResponse.infoCalled.ip, called->socket.remote_endpoint().address().to_string().c_str());
    subResponse.infoCalled.port = called->socket.remote_endpoint().port();
    subResponse.infoCaller.id = caller->id;
    std::strcpy(subResponse.infoCaller.ip, caller->socket.remote_endpoint().address().to_string().c_str());
    subResponse.infoCaller.port = caller->socket.remote_endpoint().port();



    std::cout << "IP CALLER :" << subResponse.infoCaller.ip << std::endl;
    std::cout << "PORT CALLER :" << subResponse.infoCaller.port << std::endl;
    std::cout << "IP CALLED :" << subResponse.infoCalled.ip << std::endl;
    std::cout << "PORT CALLED :" << subResponse.infoCalled.port << std::endl;

    subResponse.accept = false;
    subResponse.whoCalled = 0;
    response.call = subResponse;
    bodyResponse.pkg.res = response;
    package.header = {TYPE::CALL_RES};
    package.body = bodyResponse;
    called->socket.write_some(asio::buffer(reinterpret_cast<char*>(&package), sizeof(Package)));
}

void AsioNetwork::acceptCallResponse(std::shared_ptr<Client> called)
{
    userResCall subResponse = {0};
    userRes response = {0};
    Package package = {DEFAULT};
    int i = 0;
    auto caller = this->clientSockets[called->package.body.pkg.req.call.idCaller]; /* Get caller structure to access to his socket*/
    /* S -> C1  */
    std::cout << "[Received] - Body" << std::endl;
    std::cout << "CALL REQUEST" << std::endl;
    std::cout << "[Body pkg] accept or not: " << called->package.body.pkg.req.call.accept << std::endl;
    std::cout << "Send package to id: " << caller->id << std::endl;

    subResponse.infoCalled.id = called->id;
    std::strcpy(subResponse.infoCalled.ip, called->socket.remote_endpoint().address().to_string().c_str());
    subResponse.infoCalled.port = called->socket.remote_endpoint().port();
    subResponse.infoCaller.id = caller->id;
    std::strcpy(subResponse.infoCaller.ip, caller->socket.remote_endpoint().address().to_string().c_str());
    subResponse.infoCaller.port = caller->socket.remote_endpoint().port();

    std::cout << "IP CALLER :" << subResponse.infoCaller.ip << std::endl;
    std::cout << "PORT CALLER :" << subResponse.infoCaller.port << std::endl;
    std::cout << "IP CALLED :" << subResponse.infoCalled.ip << std::endl;
    std::cout << "PORT CALLED :" << subResponse.infoCalled.port << std::endl;

    subResponse.accept = called->package.body.pkg.req.call.accept;
    subResponse.whoCalled = 1;

    response.call = subResponse;

    package.body.pkg.res = response;
    package.header = {TYPE::CALL_RES};
    caller->socket.write_some(asio::buffer(reinterpret_cast<char*>(&package), sizeof(Package)));
}

void AsioNetwork::receiveAudio(std::shared_ptr<Client> client)
{
    std::shared_ptr<Client> clientCalled = this->clientSockets[client->package.body.pkg.req.audio.toID];

    if (!clientCalled->isConnected) {
        std::cerr << "Client [" << client->id << "] is trying to communicate with disconnected client [" << clientCalled->id << "]" << std::endl;
        return;
    }
    std::cout << "[Received] - Body" << std::endl;
    std::cout << "AUDIO REQUEST" << std::endl;
    std::cout << "to ID: " << client->package.body.pkg.req.audio.toID << std::endl;
    std::cout << "encodedPacket.bytes: " << client->package.body.pkg.req.audio.encodedPacket.bytes << std::endl;

    Package package = {DEFAULT};
    package.header = {TYPE::AUDIO_RES};
    package.body.pkg.res.audio = { client->id, client->package.body.pkg.req.audio.encodedPacket };
    clientCalled->socket.write_some(asio::buffer(reinterpret_cast<char*>(&package), sizeof(Package)));
}

void AsioNetwork::userListResponse(std::shared_ptr<Client> client)
{
    userResList list = {0};
    int index = 0;

    for (auto &currClient : this->clientSockets) {
        if (currClient->id != client->id) {
            list.userList[index] = currClient->id;
            index++;
        }
    }
    list.nbUser = this->clientSockets.size() - 1;
    
    std::cout << "User list on the server: " << std::endl;
    for (int i = 0; i < list.nbUser; i += 1)
        std::cout << " - " << list.userList[i] << " - " << std::endl;
    std::cout << std::endl;

    Package package = {DEFAULT};
    package.header = {LIST_RES};
    package.body.pkg.res.list = { list };
    client->socket.write_some(asio::buffer(reinterpret_cast<char*>(&package), sizeof(Package)));
}

void AsioNetwork::userHangUpResponse(std::shared_ptr<Client> client)
{
    std::shared_ptr<Client> clientToHangUp = this->clientSockets[client->package.body.pkg.req.hang.idToHang];
    Package package = {DEFAULT};

    package.header = {HANG_RES};
    clientToHangUp->socket.write_some(asio::buffer(reinterpret_cast<char*>(&package), sizeof(Package)));
}

void AsioNetwork::handlePackage(std::shared_ptr<Client> client)
{
    static std::unordered_map<TYPE, std::function<void(std::shared_ptr<Client> client)>> torchBearer {
        {AUDIO_REQ, [this](auto client) {this->receiveAudio(client);}},
        {CALL_REQ, [this](auto client) {
            client->package.body.pkg.req.call.whoCalled == 0 ?
            callResponse(client) :
            acceptCallResponse(client);
        }},
        {LIST_REQ, [this](auto client) { userListResponse(client); } },
        {HANG_REQ, [this](auto client) { userHangUpResponse(client);} },
    };

    if (torchBearer.find(client->package.header.type) == torchBearer.end())
        throw TorchBearerError("Unknown package type -> " + std::to_string(client->package.header.type), "Server");
    torchBearer[client->package.header.type](client);

    std::cout << "[Received] - Header";
    std::cout << std::endl << "Type: " << client->package.header.type << std::endl;
    std::cout << "Size: " << sizeof(Package) << std::endl;
}

/**************************************************************************/

void AsioNetwork::setSocketRead(std::shared_ptr<Client> client)
{
    asio::async_read(client->socket, asio::buffer(&client->package, sizeof(Package)),
            [client, this](std::error_code ec, std::size_t length) {
            if (!ec) {
                handlePackage(client);
                std::memset(&client->package, 0, sizeof(Package));
            } else {
                if (ec != asio::error::eof)
                    std::cerr << "Error while receiving package: " << ec.message() << std::endl;
                std::cout << "Disconnect client [" << client->id << "]" << std::endl;
                client->socket.close();
                client->isConnected = false;
                return;
            }
            this->setSocketRead(client);
        }
    );
}