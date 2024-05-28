cmake_minimum_required(VERSION 3.28)
project(rtsp)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)

add_executable(rtsp src/main.cpp
        lib/net/rtsp/rtsp_socket.cpp
        lib/net/rtsp/rtsp_socket.h
        lib/net/rtsp/rtsp_packet.cpp
        lib/net/rtsp/rtsp_packet.h
        lib/lib_config.h)
