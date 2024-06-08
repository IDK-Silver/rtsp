//
// Created by idk on 2024/6/2.
//

#include "rtp_packet.h"


RTPPacket::Header::Header(uint16_t seq, uint32_t timestamp, uint32_t ssrc) {
    this->version = RTP_VERSION;
    this->padding = 0;
    this->extension = 0;
    this->csrcCount = 0;

    this->marker = 0;
    this->payloadType = RTPPayloadType::JPEG;

    this->seq = htons(seq);
    this->timestamp = htonl(timestamp);
    this->ssrc = htonl(ssrc);
}

void RTPPacket::Header::set_timestamp(uint32_t timestamp) {
    this->timestamp = htonl(timestamp);
}

void RTPPacket::Header::set_ssrc(uint32_t ssrc) {
    this->ssrc = htonl(ssrc);
}

void RTPPacket::Header::set_seq(uint32_t seq) {
    this->seq = htons(seq);
}

uint32_t RTPPacket::Header::get_timestamp() const {
    return ntohl(this->timestamp);
}

uint32_t RTPPacket::Header::get_seq() const {
    return ntohs(this->seq);
}



std::array<uint8_t, 12> RTPPacket::Header::encode() const {
    std::array<uint8_t, RTP_HEADER_SIZE> buffer{};
    buffer[0] = (version << 6) | (padding << 5) | (extension << 4) | csrcCount;
    buffer[1] = (marker << 7) | payloadType;
    buffer[2] = seq >> 8;
    buffer[3] = seq & 0xFF;
    buffer[4] = timestamp >> 24;
    buffer[5] = (timestamp >> 16) & 0xFF;
    buffer[6] = (timestamp >> 8) & 0xFF;
    buffer[7] = timestamp & 0xFF;
    buffer[8] = ssrc >> 24;
    buffer[9] = (ssrc >> 16) & 0xFF;
    buffer[10] = (ssrc >> 8) & 0xFF;
    buffer[11] = ssrc & 0xFF;
    return buffer;
}


RTPPacket::RTPPacket(const Header &header) : header(header) {
    this->seq = header.get_seq();
    this->timestamp = header.get_timestamp();
}

void RTPPacket::load_video_data(const std::vector<uint8_t> &video_data, uint64_t bias) {

    if (video_data.size() + bias > this->payload.size()) {
        std::cerr << "RTPPacket : load_video_data : out of payload range" << std::endl;
        exit(-1);
    }


    for (uint64_t i = bias; i < video_data.size() + bias; i++) {
        this->payload.at(i) = video_data.at(i);
    }

    this->payload_size = video_data.size() + bias;
}

std::string RTPPacket::encode() {
    std::string result;

    for (auto data : this->header.encode()) {
        result += static_cast<char>(data);
    }

    for (uint64_t i = 0; i < this->payload_size; i++) {
        result += static_cast<char>(this->payload.at(i));
    }

    return  result;
}

void RTPPacket::set_seq(uint32_t seq) {
    this->header.set_seq(seq);
    this->seq = seq;
}

void RTPPacket::set_timestamp(uint32_t timestamp) {
    this->header.set_timestamp(timestamp);
    this->timestamp = timestamp;
}

uint32_t RTPPacket::get_seq() {
    return this->seq;
}

uint32_t RTPPacket::get_timestamp() {
    return this->timestamp;
}

