// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#include "telemetry_task.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <cstdio>

// Global queue handle definition
QueueHandle_t telemetryQueue = nullptr;

TelemetryTask::TelemetryTask()
    : Task("telemetry", tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 2) {}

void TelemetryTask::run() {
    for (int i = 0; i < 5; ++i) {
        TelemetryMessage msg{};
        // Produce a simple JSON string manually to avoid dynamic allocations
        std::snprintf(msg.payload, sizeof(msg.payload),
                      "{\"timestamp\":%d,\"altitude\":%d,\"velocity\":%d}",
                      i, 1000 + i * 10, 50 + i);

        xQueueSend(telemetryQueue, &msg, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Clean up when the task completes
    vTaskDelete(nullptr);
}

