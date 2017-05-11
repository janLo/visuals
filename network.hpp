#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

class Network {
public:
    Network();
    ~Network();

    void connect(const std::string& host, int port);
    void send(const std::vector<char> message);
    std::vector<char> recv();

private:
    WSADATA m_wsa;
    int m_socket = 0;
    sockaddr_in m_sockaddr_in;;
};

#endif