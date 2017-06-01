#define NOMINMAX
#include "network.hpp"

#include <sstream>
#include <stdexcept>
#include <algorithm>

#ifndef WIN32
    #include <cerrno>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #define SOCKET_ERROR -1
#else
    #include <WS2tcpip.h>
#endif

#pragma comment(lib,"ws2_32.lib") //Winsock Library

Network::Network()
{
    m_sockaddr_in = { 0 };

#ifdef WIN32
    // initialise winsock
    if (WSAStartup(MAKEWORD(2,2), &m_wsa) != 0)
    {
        std::stringstream ss;
        ss << "Error: WSAStartup failed: " << WSAGetLastError();
        throw std::runtime_error(ss.str());
    }
#endif

    //create socket
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        std::stringstream ss;
        ss << "Error: socket() failed: " << errno;
        throw std::runtime_error(ss.str());
    }
}

Network::~Network()
{
#ifdef WIN32
    closesocket(m_socket);
    WSACleanup();
#else
    close(m_socket);
#endif
}

void Network::connect(const std::string& host, int port)
{
    m_sockaddr_in = { 0 };
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_port = htons(port);
    m_sockaddr_in.sin_addr.s_addr = inet_addr(host.c_str());
}

void Network::send(const std::vector<char> message)
{
    if (sendto(m_socket, message.data(), message.size(), 0, (sockaddr*)&m_sockaddr_in, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        std::stringstream ss;
        ss << "Error: sendto() failed: " << errno;
        throw std::runtime_error(ss.str());
    }
}

void Network::send(const std::vector<char> message, size_t offset, size_t count)
{
    offset = std::min(message.size() - 1, offset);
    count = std::min(message.size() - offset, count);
    if (sendto(m_socket, message.data() + offset, count, 0, (sockaddr*)&m_sockaddr_in, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        std::stringstream ss;
        ss << "Error: sendto() failed: " << errno;
        throw std::runtime_error(ss.str());
    }
}

std::vector<char> Network::recv()
{
    std::vector<char> buffer;
    buffer.resize(512);
    //try to receive some data, this is a blocking call
    socklen_t len;
    if (recvfrom(m_socket, buffer.data(), buffer.size(), 0, (sockaddr*)&m_sockaddr_in, &len) == SOCKET_ERROR)
    {
        std::stringstream ss;
        ss << "Error: recvfrom() failed: " << errno;
        throw std::runtime_error(ss.str());
    }
    return buffer;
}
