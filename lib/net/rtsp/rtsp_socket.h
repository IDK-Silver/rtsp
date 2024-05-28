//
// Created by idk on 2024/5/27.
//

#ifndef RTSP_SOCKET_H
#define RTSP_SOCKET_H


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

class RTSPSocket {
public:
    RTSPSocket(const std::string &ip, uint16_t port);
    ~RTSPSocket() = default;
    bool init();
    [[nodiscard]] RTSPSocket wait_accept() const;
    static std::string wait_recv(const RTSPSocket &client_socket);

private:
    std::string ip;
    uint16_t port{};
    int socck_fd{};
    static int socket_create(int domain, int type, int protocol = 0);
    static bool socket_bind(int sockfd, const char *IP, uint16_t port);
    static bool sock_init(int rtspSockfd, const char *IP, uint16_t port, int64_t ListenQueue = 5);
};



#endif //RTSP_SOCKET_H
