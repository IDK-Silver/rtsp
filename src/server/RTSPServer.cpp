//
// Created by idk on 2024/5/29.
//

#include "RTSPServer.h"

#include <filesystem>

#define fps 30
#define ssrcNum 1233

RTSPServer::RTSPServer(uint16_t rtsp_port) {
    // create rtsp socket and init it
    this->rtsp_socket = std::make_shared<RTSPSocket>("127.0.0.1", rtsp_port);
    this->rtp_port = rtsp_port + 1;
    rtsp_socket->init();
}

void RTSPServer::run(const std::string &session) {
    this->session = session;
    while (true) {
        // wait new client connet
        auto client_socket = (this->rtsp_socket->wait_accept());

        // check client socket is valid
        if (!client_socket->is_valid()) {
            break;
        }

        // process the client socket in a separate thread
        std::thread client_thread(&RTSPServer::process_client_socket, this, client_socket);
        // Detach the thread to run independently
        client_thread.detach();
    }
}

void RTSPServer::process_client_socket(std::shared_ptr<RTSPSocket> rtsp_socket) {

    std::cout << "create new thread : process from "
              << rtsp_socket->get_ip() << ":" << rtsp_socket->get_port() << std::endl;

    RTSPPacket::Methods current_status = RTSPPacket::UNKNOW;

    // create RTP Socket
    std::shared_ptr<RTPSocket> rtp_socket = std::make_shared<RTPSocket>(
        rtsp_socket->get_ip(), this->rtp_port
    );
    rtp_socket->init(false);


    std::filesystem::path media;


    // basic info
    unsigned int  CSeq = 1;
    auto sleep_period = static_cast<uint32_t>(1000 * 1000 / fps);
    uint32_t timestamp = 0;

    while (true) {

        RTSPPacket recv_packet, send_packet;
        std::string recv = RTSPSocket::wait_recv(*rtsp_socket);
        recv_packet.decode(recv);

        std::cout << recv_packet;

        // set SCeq and Session
        send_packet.set_header_value(
            RTSPPacket::Headers::CSeq,
            std::to_string(CSeq)
        );

        send_packet.set_header_value(
            RTSPPacket::Headers::Session,
            this->session
        );

        // get method
        const auto method_string = recv_packet.get_header_value(RTSPPacket::Headers::Method);


        // process each mthod
        switch (RTSPPacket::string_to_method(method_string)) {

            case RTSPPacket::Methods::SETUP:
                current_status = RTSPPacket::Methods::SETUP;
                pcoess_rtsp_setup(send_packet, recv_packet, media);
                break;

            case RTSPPacket::Methods::PLAY:
                current_status = RTSPPacket::Methods::PLAY;
                pcoess_rtsp_play(
                    rtp_socket, send_packet, recv_packet,
                    current_status, sleep_period, timestamp
                );
                break;

            case 

            default:
                break;
        }

        auto send_str = send_packet.encode();
        if (!rtsp_socket->send_packet(send_str)) {
            std::cerr << "RTSP Server : send pacaket error " << std::endl;
        }
        CSeq++;
    }

    std::cout << "end thread : disconnet from "
              << rtsp_socket->get_ip() << ":" << rtsp_socket->get_port() << std::endl;
}

void RTSPServer::pcoess_rtsp_setup(RTSPPacket &send_packet, RTSPPacket &recv_packet,
    std::filesystem::path &media) {

    media = std::filesystem::path(recv_packet.get_filename_from_url());

    send_packet.set_header_value(
        RTSPPacket::Headers::Version,
        "RTSP/1.0"
    );

    send_packet.set_header_value(
        RTSPPacket::Headers::Status_Code,
        "200 OK"
    );

}

void RTSPServer::pcoess_rtsp_play(
    std::shared_ptr<RTPSocket> socket, RTSPPacket &send_packet, RTSPPacket &recv_packet,
    const RTSPPacket::Methods &status, const uint32_t &sleep_period, const uint32_t &timestamp
) {

    RTPPacket::Header rtp_header(0, 0, ssrcNum);

    RTPPacket packet(rtp_header);


}
