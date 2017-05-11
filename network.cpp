#include "network.hpp"

#include <sstream>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

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
    closesocket(m_socket);
#ifdef WIN32
    WSACleanup();
#endif
}

void Network::connect(const std::string& host, int port)
{
    m_sockaddr_in = { 0 };
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_port = htons(port);
    m_sockaddr_in.sin_addr.S_un.S_addr = inet_addr(host.c_str());
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

std::vector<char> Network::recv()
{
    std::vector<char> buffer;
    buffer.resize(512);
    //try to receive some data, this is a blocking call
    int len;
    if (recvfrom(m_socket, buffer.data(), buffer.size(), 0, (sockaddr*)&m_sockaddr_in, &len) == SOCKET_ERROR)
    {
        std::stringstream ss;
        ss << "Error: recvfrom() failed: " << errno;
        throw std::runtime_error(ss.str());
    }
    return buffer;
}
