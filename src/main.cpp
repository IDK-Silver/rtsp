#include <iostream>
#include <net/rtsp/rtsp_socket.h>
#include <net/rtsp/rtsp_packet.h>
#include <iostream>
#include <string>
#include <regex>
#include "./server/RTSPServer.h"
int main() {

    RTSPServer server(5002);

    server.run("4545");


    // RTSPSocket server_socket("127.0.0.1", 5251);
    //
    // server_socket.init();
    //
    // auto client_socket = server_socket.wait_accept();
    //
    // for (int i = 0; i < 1; i++) {
    //     auto recv = RTSPSocket::wait_recv(client_socket);
    //
    //     for (const char &c : recv) {
    //         if (c == '\r')
    //             std::cout << R"(\r)";
    //         else if (c == '\n')
    //             std::cout << R"(\n)";
    //         else
    //             std::cout << c;
    //     }
    //     std::cout << "\n\n";
    //
    //     RTSPPacket packet;
    //     packet.decode(recv);
    //
    //     std::cout << "method : " << packet.get_header_value(RTSPPacket::Headers::Method) << "\n";
    //
    //     if (packet.get_header_value(RTSPPacket::Headers::Method) == RTSPPacket::method_to_string(RTSPPacket::SETUP)) {
    //         std::cout << packet.get_header_value(RTSPPacket::Headers::Url) << "\n";
    //         std::cout << packet.get_header_value(RTSPPacket::Headers::CSeq) << "\n";
    //     }
    //
    //
    //     std::cout << "\npacker info\n" << packet;
    //
    //
    //
    //
    // }

    // // RTSP request message including SDP content
    // std::string rtspRequest =
    //     "ANNOUNCE rtsp://example.com/media.mp4 RTSP/1.0\r\n"
    //     "CSeq: 7\n"
    //     "Date: 23 Jan 1997 15:35:06 GMT\n"
    //     "Session: 12345678\n"
    //     "Content-Type: application/sdp\n"
    //     "Content-Length: 332\n"
    //     "\n"
    //     "v=0\n"
    //     "o=mhandley 2890844526 2890845468 IN IP4 126.16.64.4\n"
    //     "s=SDP Seminar\n"
    //     "i=A Seminar on the session description protocol\n"
    //     "u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\n"
    //     "e=mjh#isi.edu (Mark Handley)\n"
    //     "c=IN IP4 224.2.17.12/127\n"
    //     "t=2873397496 2873404696\n"
    //     "a=recvonly\n"
    //     "m=audio 3456 RTP/AVP 0\n"
    //     "m=video 2232 RTP/AVP 31";
    //
    // std::istringstream requestStream(rtspRequest);
    // std::string line;
    // std::string method, url, version;
    //
    // // Read the request line
    // std::getline(requestStream, line);
    // std::istringstream lineStream(line);
    // lineStream >> method >> url >> version;
    //
    // std::cout << "Method: " << method << std::endl;
    // std::cout << "URL: " << url << std::endl;
    // std::cout << "Version: " << version << std::endl;
    //
    // // Map to hold the headers
    // std::map<std::string, std::string> headers;
    //
    // // Read the headers
    // while (std::getline(requestStream, line) && !line.empty()) {
    //     std::istringstream headerStream(line);
    //     std::string key, value;
    //     std::getline(headerStream, key, ':');
    //     std::getline(headerStream, value);
    //     value.erase(0, value.find_first_not_of(" "));
    //     headers[key] = value;
    // }
    //
    // // Print headers
    // for (const auto& header : headers) {
    //     std::cout << "Header [" << header.first << "]: " << header.second << std::endl;
    // }
    //
    // // Reading the SDP content
    // std::string sdpContent;
    // while (std::getline(requestStream, line)) {
    //     sdpContent += line + "\n";
    // }
    //
    // std::cout << "SDP Content:\n" << sdpContent << std::endl;
    return 0;
}
