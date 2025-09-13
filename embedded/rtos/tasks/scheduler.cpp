#include "scheduler.hpp"

#include <FreeRTOS.h>
#include <task.h>

void Scheduler::add_task(Task task) {
    tasks_.push_back(task);
}

void Scheduler::start() {
    for (auto &t : tasks_) {
        TaskHandle_t handle = nullptr;
        xTaskCreate(t.fn, t.name, t.stackDepth, t.param, t.priority, &handle);
        handles_.push_back(handle);
    }
    vTaskStartScheduler();
}

void Scheduler::stop() {
    for (auto handle : handles_) {
        if (handle != nullptr) {
            vTaskDelete(handle);
        }
    }
    handles_.clear();
}

// Example heartbeat task for demonstration when SCHEDULER_DEMO is defined.
#ifdef SCHEDULER_DEMO
#include <cstdio>
static void heartbeat(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        printf("tick\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() {
    Scheduler sched;
    sched.add_task({"heartbeat", heartbeat, nullptr, tskIDLE_PRIORITY + 1,
                    configMINIMAL_STACK_SIZE});
    sched.start();
    // vTaskStartScheduler does not return under normal circumstances.
    for (;;) {}
}
#endif

