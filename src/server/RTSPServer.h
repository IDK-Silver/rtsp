//
// Created by idk on 2024/5/29.
//

#ifndef RTSPSERVER_H
#define RTSPSERVER_H

#include <net/rtsp/rtsp_socket.h>
#include <net/rtsp/rtsp_packet.h>

#include <memory>
#include <thread>
#include <net/rtp/rtp_packet.h>
#include <net/rtp/rtp_socket.h>

#include <filesystem>

class RTSPServer {
public:
    RTSPServer(uint16_t rtsp_port);
    ~RTSPServer() = default;
    void run(const std::string &session);

private:
    std::string session;
    std::shared_ptr<RTSPSocket> rtsp_socket;
    void process_client_socket(std::shared_ptr<RTSPSocket>);

    uint16_t rtp_port;
    void pcoess_rtsp_setup(RTSPPacket &send_packet, RTSPPacket &recv_packet, std::filesystem::path &media);
    void pcoess_rtsp_play (
        std::shared_ptr<RTPSocket> socket, RTSPPacket &send_packet, RTSPPacket &recv_packet,
        const RTSPPacket::Methods &status, const uint32_t &sleep_period,
        const uint32_t &timestamp
    );


    static void rend_rtp_packet()
};



#endif //RTSPSERVER_H
