#pragma once

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <cstdint>

#include "task.hpp"

/**
 * Lightweight telemetry message passed between FreeRTOS tasks via a queue.
 * Using a plain C struct keeps the payload trivially copyable for safe
 * transport through the kernel's queues.
 */
struct TelemetryMessage {
    char payload[64];
};

/** Global queue used by producer and consumer tasks */
extern QueueHandle_t telemetryQueue;

/**
 * Producer task emitting telemetry samples.
 * Demonstrates inheritance by deriving from Task and overriding run().
 */
class TelemetryTask : public Task {
public:
    TelemetryTask();
protected:
    void run() override;
};

