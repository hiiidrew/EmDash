// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#include "dis_encoder.hpp"

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>

namespace dis {

std::vector<std::uint8_t> encodeTelemetry(const TelemetryMessage &msg) {
    int timestamp = 0;
    int altitude = 0;
    int velocity = 0;

    // Parse the simple JSON string produced by TelemetryTask
    std::sscanf(msg.payload,
                "{\"timestamp\":%d,\"altitude\":%d,\"velocity\":%d}",
                &timestamp, &altitude, &velocity);

    std::vector<std::uint8_t> buffer(sizeof(std::uint32_t) * 3);
    std::uint32_t fields[3];
    fields[0] = htonl(static_cast<std::uint32_t>(timestamp));
    fields[1] = htonl(static_cast<std::uint32_t>(altitude));
    fields[2] = htonl(static_cast<std::uint32_t>(velocity));

    std::memcpy(buffer.data(), fields, sizeof(fields));
    return buffer;
}

} // namespace dis

