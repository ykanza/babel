#ifndef NETWORK_H
#define NETWORK_H

#include <asio.hpp>
#include <list>
#include <memory>
#include <utility>
#include "../include/protocolManager.hpp"
#include "Inetwork.hpp"
#include "Client.hpp"

class AsioNetwork : public InetworkServer
{
private:
    asio::io_context m_asioContext;
    std::thread m_threadContext;
    asio::ip::tcp::acceptor m_asioAcceptor;
    std::vector<std::shared_ptr<Client>> clientSockets;
public:
    explicit AsioNetwork(uint16_t port);
    ~AsioNetwork() override;
    
    /*  Async method */
    void getNewClientConnections() override;
    void setSocketRead(std::shared_ptr<Client> client) override;
    /*****************/

    void openConnections() override;
    void closeConnections() override;

    /*  Package response method */
    void callResponse(std::shared_ptr<Client> caller) override;
    void acceptCallResponse(std::shared_ptr<Client> client) override;
    void receiveAudio(std::shared_ptr<Client> client) override;
    void userListResponse(std::shared_ptr<Client> client) override;
    void userHangUpResponse(std::shared_ptr<Client> client) override;
    void handlePackage(std::shared_ptr<Client> client) override;
    /****************************/

};

#endif