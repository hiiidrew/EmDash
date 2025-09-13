// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#include "telemetry_task.hpp"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <cstdio>

// Global queue handle definition
QueueHandle_t telemetryQueue = nullptr;

void telemetry_producer_task(void *pvParameters) {
    (void)pvParameters;

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

