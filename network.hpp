#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <vector>
#include <string>

#ifdef WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <winsock2.h>
#else
    #include <netinet/ip.h>
#endif

class Network {
public:
    Network();
    ~Network();

    void connect(const std::string& host, int port);
    void send(const std::vector<char> message);
    void send(const std::vector<char> message, size_t offset, size_t count);
    std::vector<char> recv();

private:
#ifdef WIN32
    WSADATA m_wsa;
#endif
    int m_socket = 0;
    sockaddr_in m_sockaddr_in;;
};

#endif
