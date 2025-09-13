#include <FreeRTOS.h>
#include <queue.h>
#include <iostream>
#include <task.h>

#include "rtos/tasks/telemetry_task.hpp"
#include "rtos/tasks/ai_task.hpp"
#include "rtos/tasks/scheduler.hpp"

/**
 * Entry point for the simulated embedded system. Tasks are created and the
 * FreeRTOS scheduler is started to orchestrate execution.
 */
int main() {
    std::cout << "Starting EmDash Embedded Simulation" << std::endl;

    telemetryQueue = xQueueCreate(10, sizeof(TelemetryMessage));

    TelemetryTask telemetry;
    AITask ai;

    Scheduler sched;
    sched.add_task(&telemetry);
    sched.add_task(&ai);

    // Start tasks running concurrently
    sched.start();

    // Allow tasks to execute
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Stop scheduler and join task threads
    sched.stop();

    std::cout << "Simulation complete" << std::endl;
    return 0;
}

