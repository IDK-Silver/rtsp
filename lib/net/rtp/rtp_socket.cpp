//
// Created by idk on 2024/6/3.
//

#include "rtp_socket.h"


RTPSocket::RTPSocket(const std::string &ip, const uint16_t port) {

    this->ip = ip;
    this->port = port;
}

RTPSocket::~RTPSocket() {
    close(this->socck_fd);
}



bool RTPSocket::init(bool bind) {
    this->socck_fd = socket_create(AF_INET, SOCK_DGRAM);

    if (!bind)
        return false;

    if (!sock_init(this->socck_fd, this->ip.c_str(), this->port, SOMAXCONN))
        return false;

    return true;
}

bool RTPSocket::send_packet(RTPPacket &packet) {

    sockaddr_in addr{};
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    auto rtp_data = packet.encode();

    auto ret = sendto(
        this->socck_fd, rtp_data.c_str(), rtp_data.length(), 0,
        reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr)
    );

    if (ret < 0)
        fprintf(stderr, "RTP Socket send_packet failed: %s\n", strerror(errno));

    this->timestamp += this->each_timestamp;
    this->seq++;

    return ret;
}


int RTPSocket::socket_create(int domain, int type, int protocol) {
    int fd = socket(domain, type, protocol);

    if (fd < 0) {
        fprintf(stderr, "RTSP::Socket() failed: %s\n", strerror(errno));
        return fd;
    }

    return fd;
}

bool RTPSocket::socket_bind(int sockfd, const char *IP, const uint16_t port) {
    sockaddr_in addr{};
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, IP, &addr.sin_addr);
    if (bind(sockfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        fprintf(stderr, "bind() failed: %s\n", strerror(errno));
        return false;
    }
    return true;
}


bool RTPSocket::sock_init(int rtspSockfd, const char *IP, const uint16_t port, const int64_t ListenQueue)
{
    if (!socket_bind(rtspSockfd, IP, port))
        return false;
    // noinspection SameParameterValue
    if (listen(rtspSockfd, ListenQueue) < 0)
    {
        fprintf(stderr, "listen() failed: %s\n", strerror(errno));
        return false;
    }
    return true;
}

