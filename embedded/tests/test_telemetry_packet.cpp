#include <cassert>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>

#include "../protocols/dis_encoder.hpp"
#include "../rtos/tasks/telemetry_task.hpp"

int main() {
    TelemetryMessage msg;
    std::snprintf(msg.payload, sizeof(msg.payload),
                  "{\"timestamp\":1,\"altitude\":100,\"velocity\":50}");

    auto encoded = dis::encodeTelemetry(msg);
    assert(encoded.size() == sizeof(std::uint32_t) * 3);

    std::uint32_t fields[3];
    std::memcpy(fields, encoded.data(), encoded.size());

    assert(ntohl(fields[0]) == 1);
    assert(ntohl(fields[1]) == 100);
    assert(ntohl(fields[2]) == 50);
    return 0;
}
