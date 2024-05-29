//
// Created by idk on 2024/5/29.
//

#include "RTSPServer.h"

#include <filesystem>


RTSPServer::RTSPServer(uint16_t rtsp_port, uint16_t rtp_port) {


    // create rtsp socket and init it
    this->rtsp_socket = std::make_shared<RTSPSocket>("127.0.0.1", rtsp_port);
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
        client_thread.detach();  // Detach the thread to run independently
    }
}

void RTSPServer::process_client_socket(std::shared_ptr<RTSPSocket> socket) {

    std::cout << "create new thread : process from "
              << socket->get_ip() << ":" << socket->get_port() << std::endl;


    std::filesystem::path media;
    unsigned int  CSeq = 1;
    while (true) {

        RTSPPacket recv_packet, send_packet;
        std::string recv = RTSPSocket::wait_recv(*socket);
        recv_packet.decode(recv);


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
                pcoess_rtsp_setup(socket,send_packet, recv_packet, media);
            break;

            default:
                break;

        }



        auto send_str = send_packet.encode();
        socket->send_packet(send_str);
        CSeq++;
    }

    std::cout << "end thread : disconnet from "
              << socket->get_ip() << ":" << socket->get_port() << std::endl;
}

void RTSPServer::pcoess_rtsp_setup(std::shared_ptr<RTSPSocket> socket, RTSPPacket &send_packet, RTSPPacket &recv_packet,
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
