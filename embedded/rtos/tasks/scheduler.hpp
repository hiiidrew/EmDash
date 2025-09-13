#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include <vector>

/**
 * Lightweight wrapper around the FreeRTOS scheduler to simplify
 * task registration from C++ code.
 */
class Scheduler {
public:
    struct Task {
        const char *name;
        TaskFunction_t fn;
        void *param;
        UBaseType_t priority;
        uint16_t stackDepth;
    };

    void add_task(Task task);
    void start();
    void stop();

private:
    std::vector<Task> tasks_;
    std::vector<TaskHandle_t> handles_;
};

