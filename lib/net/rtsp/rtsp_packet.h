//
// Created by idk on 2024/5/27.
//

#ifndef RTSP_PACKET_H
#define RTSP_PACKET_H
#include <lib_config.h>
#include <iostream>
#include <regex>
#include <string>


class RTSPPacket {
public:

    enum Methods {
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

    enum Headers {
        Method,
        Url,
        Version,
        CSeq,
        ContentLength,
        ContentType,
        Date,
        Session,
        SDP,
        Unknow
    };

    static std::string method_to_string(Methods method);
    static Methods string_to_method(const std::string &method);

    static std::string header_to_string(Headers header);
    static Headers string_to_header(const std::string &header);
    RTSPPacket() = default;
    ~RTSPPacket() = default;


    bool decode(const std::string &request);

    std::string get_header_value(Headers header);
private:

    std::map<Headers, std::string> headers;
};



#endif //RTSP_PACKET_H
