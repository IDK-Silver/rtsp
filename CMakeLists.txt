cmake_minimum_required(VERSION 3.0)
project(rtsp)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)

add_executable(rtsp
        src/main.cpp
        lib/net/rtsp/rtsp_socket.cpp
        lib/net/rtsp/rtsp_socket.h
        lib/net/rtsp/rtsp_packet.cpp
        lib/net/rtsp/rtsp_packet.h
        lib/lib_config.h
        src/server/RTSPServer.cpp
        src/server/RTSPServer.h
        lib/net/rtp/rtp_packet.cpp
        lib/net/rtp/rtp_packet.h
        lib/net/rtp/rtp_packet.h
        lib/net/rtp/rtp_socket.cpp
        lib/net/rtp/rtp_socket.h
        lib/net/basic.h)
