//
// Created by idk on 2024/5/27.
//

#include "rtsp_packet.h"
#include <vector>

std::string RTSPPacket::method_to_string(Methods method) {

    switch (method) {
        case OPTIONS:
            return "OPTION";

        case DESCRIBE:
            return "DESCRIBE";

        case SETUP:
            return "SETUP";

        case PLAY:
            return "PLAY";

        case PAUSE:
            return "PAUSE";

        case RECORD:
            return "RECORD";

        case ANNOUNCE:
            return "ANNOUNCE";

        case TEARDOWN:
            return "TEARDOWN";

        case GET_PARAMETER:
            return "GET_PARAMETER";

        case SET_PARAMETER:
            return "SET_PARAMETER";

        case REDIRECT:
            return "REDIRECT";

        default:
            return "UNKNOW";
    }
}

RTSPPacket::Methods RTSPPacket::string_to_method(const std::string& method) {

    std::vector<Methods> all_method = {
        OPTIONS, DESCRIBE, SETUP, PLAY, PAUSE,
        PAUSE, RECORD, ANNOUNCE, TEARDOWN, GET_PARAMETER,
        SET_PARAMETER, REDIRECT
    };


    for (const auto &target : all_method) {
        if (method == method_to_string(target))
            return target;
    }

    return UNKNOW;
}


std::string RTSPPacket::header_to_string(Headers header) {
    switch (header) {
        case Method:
            return "Method";

        case Url:
            return "Url";

        case Version:
            return "Version";

        case CSeq:
            return "CSeq";

        case ContentLength:
            return "Content-Length";

        case ContentType:
            return "Content-Type";

        case Date:
            return "Date";

        case Session:
            return "Session";

        case SDP:
            return "SDP";

        default:
            return "Unknow";
    }
}

RTSPPacket::Headers RTSPPacket::string_to_header(const std::string &header) {
    std::vector<Headers> all_headers = {
        Method, Url, Version, CSeq, ContentLength,
        ContentType, Date, Session, SDP, Unknow
    };

    for (const auto &target : all_headers) {
        if (header == header_to_string(target))
            return target;
    }

    return Unknow;
}

bool RTSPPacket::decode(const std::string &request) {

    // request to string string
    std::istringstream request_stream(request);

    // the string of basic header
    std::string line;

    // basic header info
    std::string method, url, version;

    // read the request line
    std::getline(request_stream, line);
    std::istringstream lineStream(line);
    lineStream >> method >> url >> version;


#if MESSAGE_RTSP_PACKET_DECODE
    std::cout << "Method: " << method << std::endl;
    std::cout << "URL: " << url << std::endl;
    std::cout << "Version: " << version << std::endl;
#endif


    // set basic header info
    this->headers[Headers::Method] = method;
    this->headers[Headers::Url] = url;
    this->headers[Headers::Version] = version;

    // read the headers
    while (std::getline(request_stream, line) && !line.empty()) {
        std::istringstream header_stream(line);
        std::string key, value;
        std::getline(header_stream, key, ':');
        std::getline(header_stream, value);
        value.erase(0, value.find_first_not_of(' '));
        this->headers[RTSPPacket::string_to_header(key)] = value;
    }

    // print headers
#if MESSAGE_RTSP_PACKET_DECODE
    for (const auto& header : this->headers) {
        std::cout << "Header [" << header.first << "]: " << header.second << std::endl;
    }
#endif

    // reading the SDP content
    std::string sdpContent;
    while (std::getline(request_stream, line)) {
        sdpContent += line + "\n";
    }

    // print SDP content
#if MESSAGE_RTSP_PACKET_DECODE
    std::cout << "SDP Content:\n" << sdpContent << std::endl;
#endif


    return true;
}

std::string RTSPPacket::get_header_value(Headers header) {
    return this->headers[header];
}
