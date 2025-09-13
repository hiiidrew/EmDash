#include "ai_task.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

AITask::AITask()
    : Task("ai", tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 2) {}

void AITask::run() {
    TelemetryMessage msg;
    for (;;) {
        if (xQueueReceive(telemetryQueue, &msg, portMAX_DELAY) == pdTRUE) {
            std::cout << "AI received telemetry: " << msg.payload << '\n';
            vTaskDelay(pdMS_TO_TICKS(150));
        }
    }
}

