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
    for (const auto &target : get_all_methods()) {
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

        case Status_Code:
            return "STATUSCODE";

        case Transport:
            return "Transport";

        default:
            return "Unknow";
    }
}

RTSPPacket::Headers RTSPPacket::string_to_header(const std::string &header) {


    for (const auto &target : get_all_headers()) {
        if (header == header_to_string(target))
            return target;
    }

    return Unknow;
}

std::string RTSPPacket::encode() {

    std::vector<Headers> basic_info {
        Method, Url, Version, Status_Code
    };

    std::string result;

    if (!this->get_header_value(Headers::Method).empty())
        result += this->headers[Headers::Method] + " ";

    if (!this->get_header_value(Headers::Url).empty())
        result += this->headers[Headers::Url] + " ";

    if (!this->get_header_value(Headers::Version).empty())
        result += this->headers[Headers::Version] + " ";

    if (!this->get_header_value(Headers::Status_Code).empty())
        result += this->headers[Headers::Status_Code];

    result += "\r\n";


    for (auto header : get_all_headers()) {
        if (std::find(basic_info.begin(), basic_info.end(), header) == basic_info.end()) {
            if (!this->get_header_value(header).empty())
                result += header_to_string(header) + ": " + this->headers[header] + "\r\n";
        }
    }

    return result;
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

        // remove \r \n
        while (!value.empty() &&
           (value.back() == '\r' || value.back() == '\n')) {
            value.pop_back();
           }

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

    if (this->headers.find(header) == this->headers.end()) {
        return "";
    }

    return this->headers[header];
}

std::vector<RTSPPacket::Methods> RTSPPacket::get_all_methods() {
    std::vector<Methods> all_method = {
        OPTIONS, DESCRIBE, SETUP, PLAY, PAUSE,
        PAUSE, RECORD, ANNOUNCE, TEARDOWN, GET_PARAMETER,
        SET_PARAMETER, REDIRECT
    };
    return all_method;
}

std::vector<RTSPPacket::Headers> RTSPPacket::get_all_headers() {
    std::vector<Headers> all_headers = {
        Method, Url, Version, CSeq, ContentLength,
        ContentType, Date, Session, SDP, Unknow
    };

    return all_headers;
}

std::string RTSPPacket::get_filename_from_url(const std::string &url) {
    // find the position of the last '/' in the url
    size_t lastSlashPos = url.rfind('/');
    if (lastSlashPos == std::string::npos) return ""; // if no '/' found, return an empty string

    // extract the part from the last '/' to the end of the string
    std::string filename = url.substr(lastSlashPos + 1);

    // check if there are parameters or other characters to remove
    size_t paramPos = filename.find('?');
    if (paramPos != std::string::npos) {
        filename = filename.substr(0, paramPos); // take the substring before the '?'
    }

    return filename;
}

std::string RTSPPacket::get_filename_from_url() {
    return RTSPPacket::get_filename_from_url(
        this->get_header_value(Headers::Url)
    );
}

bool RTSPPacket::set_header_value(Headers header, const std::string &value) {

    this->headers[header] = value;
    return true;
}



std::ostream& operator<<(std::ostream& os, RTSPPacket& packet)
{

    for (const auto& header : RTSPPacket::get_all_headers()) {
        os  << "Header [" << RTSPPacket::header_to_string(header) << "]: "
            << packet.get_header_value(header) << std::endl;
    }


    return os;
}
