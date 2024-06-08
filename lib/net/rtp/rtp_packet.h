//
// Created by idk on 2024/6/2.
//

#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include <lib_config.h>
#include <iostream>
#include <regex>
#include <array>
#include <string>
#include <algorithm>

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <arpa/inet.h>
#include "../basic.h"


constexpr int64_t RTP_VERSION = 2;
constexpr int64_t RTP_HEADER_SIZE = 12;
constexpr int64_t FU_Size = 2;
constexpr int64_t RTP_MAX_DATA_SIZE = MAX_UDP_PACKET_SIZE - UDP_HEADER_SIZE - IP_HEADER_SIZE - RTP_HEADER_SIZE - FU_Size;
constexpr int64_t RTP_MAX_PACKET_LEN = RTP_MAX_DATA_SIZE + RTP_HEADER_SIZE + FU_Size;
constexpr int64_t RTP_MAX_PAYLOAD_SIZE = RTP_MAX_DATA_SIZE + FU_Size;


enum RTPPayloadType {
    JPEG = 26,
    H264 = 96
};


class RTPPacket {

public:

    class Header {

    public:
        Header() = default;
        Header(uint16_t seq, uint32_t timestamp, uint32_t ssrc);
        ~Header() = default;
        void set_timestamp(uint32_t timestamp);
        void set_ssrc(uint32_t ssrc);
        void set_seq(uint32_t seq);
        [[nodiscard]] std::array<uint8_t, 12> encode() const;
        [[nodiscard]] uint32_t get_timestamp() const;
        [[nodiscard]] uint32_t get_seq() const;
    private:
        //byte 0
        uint8_t csrcCount : 4;
        uint8_t extension : 1;
        uint8_t padding : 1;
        uint8_t version : 2;
        //byte 1
        uint8_t payloadType : 7;
        uint8_t marker : 1;
        //byte 2, 3
        uint16_t seq{};
        //byte 4-7
        uint32_t timestamp{};
        //byte 8-11
        uint32_t ssrc{};
    };


    explicit RTPPacket(const Header &header);
    ~RTPPacket() = default;

    void load_video_data(const std::vector<uint8_t> &video_data, uint64_t bias = 0);


    std::string encode();

    void set_seq(uint32_t seq);
    void set_timestamp(uint32_t timestamp);

    uint32_t get_seq();
    uint32_t get_timestamp();

private:
    Header header;
    std::array<uint8_t, RTP_MAX_PAYLOAD_SIZE> payload = {0};
    uint64_t payload_size = 0;

    uint16_t seq{};
    uint32_t timestamp{};
};


#endif //RTP_PACKET_H
