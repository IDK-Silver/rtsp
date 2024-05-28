//
// Created by idk on 2024/5/27.
//

#include "rtsp_packet.h"

#include <vector>

std::string RTSPPacket::method_to_string(Method method) {

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

RTSPPacket::Method RTSPPacket::string_to_method(const std::string& method) {

    std::vector<Method> all_method = {
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
