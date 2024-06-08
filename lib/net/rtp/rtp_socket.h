//
// Created by idk on 2024/6/3.
//

#ifndef RTP_SOCKET_H
#define RTP_SOCKET_H

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <vector>

#include "rtp_packet.h"

class RTPSocket {
public:
    RTPSocket(const std::string &ip, uint16_t port);
    ~RTPSocket();

    bool init(bool bind = true);

    bool RTPSocket::send_packet(RTPPacket &packet);

private:
    std::string ip;
    uint16_t port{};
    int socck_fd{};
    static int socket_create(int domain, int type, int protocol = 0);
    static bool socket_bind(int sockfd, const char *IP, uint16_t port);
    static bool sock_init(int rtspSockfd, const char *IP, uint16_t port, int64_t ListenQueue);

    uint32_t each_timestamp;
    uint32_t seq = 0;
    uint32_t timestamp = 0;
};



#endif //RTP_SOCKET_H
