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
    void setBrightness(float brightness);

    bool handleGet(CivetServer* server, mg_connection* conn);

    int m_width = 25;
    int m_height = 20;
    int m_roof = 10;
    int m_fps = 30;
    std::string m_host = "192.168.1.111";
    int m_port = 7000;
    int m_portControl = 7001;
    float m_brightness;
    std::vector<int> m_leds;

    Network m_network;
    Network m_networkControl;
    std::unique_ptr<CivetServer> m_server;
    Sound m_sound;
    int m_streamID = 0;
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

    // init led lookup table
/*    for (int x=0, i=0; x < m_width; x++) {
        int off = (x / 5) % 2;
        for (int y=0; y < m_height; y++, i++) {
            if ((x + off) % 2 == 0)
                m_leds.push_back(x * m_height + y);
            else
                m_leds.push_back(x * m_height + (m_height - 1) - y);                
        }
    }*/

    m_leds = { 0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475 };
    for (int x=1; x<25; x++) {
        for (int y=0; y<20; y++) {
            if ((x % 2) == ((x / 5) % 2))
                m_leds.push_back(m_leds[y] + x);
            else
                m_leds.push_back(m_leds[19-y] + x);
        }
    }
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
        std::string portControl;
        if (CivetServer::getParam(conn, "portControl", portControl, 0)) {
            int portControli = atoi(portControl.c_str());
            if (portControli)
                m_portControl = portControli;
            message = "portControl=" + std::to_string(m_portControl);
        }
        std::string brightness;
        if (CivetServer::getParam(conn, "brightness", brightness, 0)) {
            float brightnessf = static_cast<float>(atof(brightness.c_str()));
            if (brightnessf) {
                m_brightness = brightnessf;
                setBrightness(m_brightness);
            }
            message = "brightness=" + std::to_string(m_brightness);
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

    for (int i=0; i < m_width * m_height; i++) {
        unsigned int col = buffer[m_leds[i]];
        out[i * 3 + 0] = (col & 0xff0000) >> 16;
        out[i * 3 + 1] = (col & 0x00ff00) >> 8;
        out[i * 3 + 2] = (col & 0x0000ff) >> 0;
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

void Visuals::setBrightness(float brightness)
{
    std::vector<char> control;
    control.push_back(2);
    control.push_back(static_cast<char>(brightness * 255.0f));
    m_networkControl.send(control);
}

int Visuals::main(int argc, char* argv[])
{
    std::vector<unsigned int> buffer;
    m_network.connect(m_host, m_port);
    m_networkControl.connect(m_host, m_portControl);

    m_streamID = m_sound.play("compo.ogg", true);
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
