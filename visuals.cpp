#include <iostream>

#include "network.hpp"
#include <thread>
#include <memory>
#include "CivetServer.h"
#include "sound.hpp"

class Visuals : public CivetHandler {
public:
    Visuals();
    int main(int argc, char* argv[]);

private:
    void send(const std::vector<unsigned int>& buffer);
    void fill(std::vector<unsigned int>& buffer);

    bool handleGet(CivetServer* server, mg_connection* conn);

    int m_width = 25;
    int m_height = 20;
    int m_fps = 60;
    std::string m_host = "192.168.17.1";
    int m_port = 7890;

    Network m_network;
    std::unique_ptr<CivetServer> m_server;
    Sound m_sound;
};

Visuals::Visuals()
{
    // start webserver
    std::string path = "";
    std::cout << "starting webserver: " << path << std::endl;
    const char* options[] = { "document_root", path.c_str(),
                              "listening_ports", "8080",
                              "num_threads", "4",
                              nullptr
                            };
    m_server.reset(new CivetServer(options));
    m_server->addHandler("/set", this);
}

bool Visuals::handleGet(CivetServer* server, mg_connection* conn)
{
    std::string uri = mg_get_request_info(conn)->uri;
    
    if (uri == "/set") {
        std::string message;
        std::string fps;
        if (CivetServer::getParam(conn, "fps", fps, 0)) {
            int fpsi = atoi(fps.c_str());
            if (fpsi)
                m_fps = fpsi;
            message = "fps=" + std::to_string(m_fps);
        }
        std::string host;
        if (CivetServer::getParam(conn, "host", host, 0)) {
            if (!host.empty())
                m_host = host;
            message = "host=" + m_host;
        }
        std::string port;
        if (CivetServer::getParam(conn, "port", port, 0)) {
            int porti = atoi(port.c_str());
            if (porti)
                m_port = porti;
            message = "port=" + std::to_string(m_port);
        }
        message = "HTTP/1.1 200 OK\r\n\r\n" + message;
        mg_printf(conn, message.c_str());
    }

    return true;
}

void Visuals::send(const std::vector<unsigned int>& buffer)
{
    std::vector<char> out;
    out.resize(m_width * m_height * 3);

    for (int x=0, i=0; x<m_width; x++) {
        if ((x & 1) == 0) {
            for (int y=0; y<m_height; y++, i+=3) {
                unsigned int col = buffer[y * m_width + x];
                out[i+0] = (col & 0x0000ff) >> 0;
                out[i+1] = (col & 0x00ff00) >> 8;
                out[i+2] = (col & 0xff0000) >> 16;
            }
        } else {
            for (int y=m_height-1; y>=0; y--, i+=3) {
                unsigned int col = buffer[y * m_width + x];
                out[i+0] = (col & 0x0000ff) >> 0;
                out[i+1] = (col & 0x00ff00) >> 8;
                out[i+2] = (col & 0xff0000) >> 16;
            }
        }
    }
    m_network.send(out, 0, 800);
    m_network.send(out, 800, 700);
}

void Visuals::fill(std::vector<unsigned int>& buffer)
{
    buffer.resize(m_width * m_height);
    for (int y=0; y<m_height; y++) {
        for (int x=0; x<m_width; x++) {

        }
    }
}

int Visuals::main(int argc, char* argv[])
{

    std::vector<unsigned int> buffer;
    m_network.connect(m_host, m_port);
    while (true) {
        buffer.clear();
        fill(buffer);
        try {
            send(buffer);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/m_fps));
        } catch (const std::runtime_error& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
    
    return 0;
}

int main(int argc, char* argv[])
{
    Visuals visuals;
    return visuals.main(argc, argv);
}
