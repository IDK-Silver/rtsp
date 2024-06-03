//
// Created by idk on 2024/5/27.
//

#include "rtsp_socket.h"

RTSPSocket::RTSPSocket(const std::string &ip, const uint16_t port) {

    this->ip = ip;
    this->port = port;
}

RTSPSocket::~RTSPSocket() {
    close(this->socck_fd);
}

bool RTSPSocket::init() {
    this->socck_fd = socket_create(AF_INET, SOCK_STREAM);

    if (!sock_init(this->socck_fd, this->ip.c_str(), this->port, SOMAXCONN))
        return false;

    return true;
}

std::shared_ptr<RTSPSocket> RTSPSocket::wait_accept() const {

    sockaddr_in cliAddr{};
    bzero(&cliAddr, sizeof(cliAddr));
    socklen_t addrLen = sizeof(cliAddr);

    const auto client_fd = accept(this->socck_fd, reinterpret_cast<sockaddr *>(&cliAddr), &addrLen);

    if (client_fd < 0)
    {
        fprintf(stderr, "accept error(): %s\n", strerror(errno));
        //continue;
        exit(-1);
    }
    char ipv4[16]{0};

    fprintf(stdout,
            "Connection from %s:%d\n",
            inet_ntop(AF_INET, &cliAddr.sin_addr, ipv4, sizeof(ipv4)),
            ntohs(cliAddr.sin_port));

    std::shared_ptr<RTSPSocket> client_socket = std::make_shared<RTSPSocket>(std::string(ipv4), ntohs(cliAddr.sin_port));
    client_socket->socck_fd = client_fd;


    return client_socket;
}

std::string RTSPSocket::wait_recv(const RTSPSocket &client_socket) {

    std::string result;

    char recv_buffer[1024]{0};

    auto recv_length = recv(client_socket.socck_fd, recv_buffer, sizeof(recv_buffer), 0);

    if (recv_length <= 0)
        return result;

    result = std::string(recv_buffer);
    // result.at(recv_length) = 0;

    return result;
}

bool RTSPSocket::send_packet(const std::string &packet) const {
    return (send(this->socck_fd, packet.c_str(), strlen(packet.c_str()), 0) < 1);
}

bool RTSPSocket::is_valid() const {
    return (this->socck_fd >= 0);
}

std::string RTSPSocket::get_ip() {
    return this->ip;
}

uint16_t RTSPSocket::get_port() {
    return this->port;
}


int RTSPSocket::socket_create(int domain, int type, int protocol) {
    int fd = socket(domain, type, protocol);

    if (fd < 0) {
        fprintf(stderr, "RTSP::Socket() failed: %s\n", strerror(errno));
        return fd;
    }

    return fd;
}

bool RTSPSocket::socket_bind(int sockfd, const char *IP, const uint16_t port) {
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


bool RTSPSocket::sock_init(int rtspSockfd, const char *IP, const uint16_t port, const int64_t ListenQueue)
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