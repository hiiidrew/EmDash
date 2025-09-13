#pragma once

#include <vector>
#include <cstdint>

#include "../rtos/tasks/telemetry_task.hpp"

namespace dis {

/**
 * Encode a telemetry message into a minimal Distributed Interactive Simulation
 * (DIS) PDU.  The resulting byte buffer stores three 32-bit big-endian fields:
 * timestamp, altitude and velocity.
 */
std::vector<std::uint8_t> encodeTelemetry(const TelemetryMessage &msg);

} // namespace dis

