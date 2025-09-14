// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "../rtos/tasks/telemetry_task.hpp"

#if __has_include(<flatbuffers/flatbuffers.h>)
#include <flatbuffers/flatbuffers.h>
#define EMDASH_HAS_FLATBUFFERS 1
#else
#define EMDASH_HAS_FLATBUFFERS 0
#endif

namespace proto {

std::vector<std::uint8_t> serializeTelemetryFlatbuffer(const TelemetryMessage &msg) {
    int timestamp = 0;
    float altitude = 0.0f;
    float velocity = 0.0f;

    std::sscanf(msg.payload,
                "{\"timestamp\":%d,\"altitude\":%f,\"velocity\":%f}",
                &timestamp, &altitude, &velocity);

#if EMDASH_HAS_FLATBUFFERS
    flatbuffers::FlatBufferBuilder builder(128);

    auto startTelemetry = builder.StartTable();
    builder.AddElement<std::uint32_t>(4, static_cast<std::uint32_t>(timestamp), 0);
    builder.AddElement<float>(6, altitude, 0.0f);
    builder.AddElement<float>(8, velocity, 0.0f);
    auto telemetry = builder.EndTable(startTelemetry, 3);

    builder.StartVector(sizeof(flatbuffers::Offset<void>), 1,
                        sizeof(flatbuffers::Offset<void>));
    builder.AddOffset(telemetry);
    auto vec = builder.EndVector(1);

    auto startRoot = builder.StartTable();
    builder.AddOffset(4, vec);
    auto root = builder.EndTable(startRoot, 1);

    builder.Finish(flatbuffers::Offset<void>(root));
    const std::uint8_t *buf = builder.GetBufferPointer();
    return {buf, buf + builder.GetSize()};
#else
    std::vector<std::uint8_t> buffer(sizeof(std::uint32_t) + sizeof(float) * 2);
    std::uint32_t ts = static_cast<std::uint32_t>(timestamp);
    std::memcpy(buffer.data(), &ts, sizeof(ts));
    std::memcpy(buffer.data() + sizeof(ts), &altitude, sizeof(float));
    std::memcpy(buffer.data() + sizeof(ts) + sizeof(float), &velocity, sizeof(float));
    return buffer;
#endif
}

} // namespace proto

