//
// Created by idk on 2024/5/29.
//

#ifndef RTSPSERVER_H
#define RTSPSERVER_H

#include <net/rtsp/rtsp_socket.h>
#include <net/rtsp/rtsp_packet.h>

#include <memory>
#include <thread>
#include <filesystem>

class RTSPServer {
public:
    RTSPServer(uint16_t rtsp_port, uint16_t rtp_port);
    ~RTSPServer() = default;
    void run(const std::string &session);

private:
    std::string session;
    std::shared_ptr<RTSPSocket> rtsp_socket;
    void process_client_socket(std::shared_ptr<RTSPSocket>);


    void pcoess_rtsp_setup(std::shared_ptr<RTSPSocket> socket, RTSPPacket &send_packet, RTSPPacket &recv_packet, std::filesystem::path &media);
};



#endif //RTSPSERVER_H
