#include <iostream>
#include <net/rtsp/rtsp_socket.h>

int main() {

    RTSPSocket server_socket("127.0.0.1", 5050);

    server_socket.init();

    auto client_socket = server_socket.wait_accept();

    for (int i = 0; i < 1; i++) {
        auto recv = RTSPSocket::wait_recv(client_socket);

        for (const char &c : recv) {
            if (c == '\r')
                std::cout << R"(\r)";
            else if (c == '\n')
                std::cout << R"(\n)";
            else
                std::cout << c;
        }
    }

    return 0;
}
