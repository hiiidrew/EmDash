#include <FreeRTOS.h>
#include <queue.h>
#include <iostream>

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

    // This call does not return under normal operation
    sched.start();

    // Should never reach here
    for (;;)
        ;
    return 0;
}

