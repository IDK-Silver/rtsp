//
// Created by idk on 2024/5/27.
//

#ifndef RTSP_PACKET_H
#define RTSP_PACKET_H
#include <string>


class RTSPPacket {
public:

    enum Method {
        OPTIONS,
        DESCRIBE,
        SETUP,
        PLAY,
        PAUSE,
        RECORD,
        ANNOUNCE,
        TEARDOWN,
        GET_PARAMETER,
        SET_PARAMETER,
        REDIRECT,
        UNKNOW
    };

    static std::string method_to_string(Method method);
    static Method string_to_method(const std::string& method);

    RTSPPacket() = default;
    ~RTSPPacket() = default;

private:

    Method method;
    std::string url;
    std::string version;

};



#endif //RTSP_PACKET_H
