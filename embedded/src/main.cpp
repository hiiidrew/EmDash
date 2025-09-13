#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <iostream>

#include "rtos/tasks/telemetry_task.hpp"
#include "rtos/tasks/scheduler.hpp"

/**
 * Entry point for the simulated embedded system. Tasks are created and the
 * FreeRTOS scheduler is started to orchestrate execution.
 */
static void ai_task(void *pvParameters) {
    (void)pvParameters;
    TelemetryMessage msg;
    for (;;) {
        if (xQueueReceive(telemetryQueue, &msg, portMAX_DELAY) == pdTRUE) {
            std::cout << "AI received telemetry: " << msg.payload << '\n';
            vTaskDelay(pdMS_TO_TICKS(150));
        }
    }
}

int main() {
    std::cout << "Starting EmDash Embedded Simulation" << std::endl;

    telemetryQueue = xQueueCreate(10, sizeof(TelemetryMessage));

    Scheduler sched;
    sched.add_task({"telemetry", telemetry_producer_task, nullptr,
                    tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 2});
    sched.add_task({"ai", ai_task, nullptr,
                    tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 2});

    // This call does not return under normal operation
    sched.start();

    // Should never reach here
    for (;;)
        ;
    return 0;
}

