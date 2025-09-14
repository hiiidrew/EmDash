#pragma once

#include "FreeRTOS.h"
#include <thread>
#include <chrono>

using TaskFunction_t = void (*)(void *);
using TaskHandle_t = std::thread*;

inline BaseType_t xTaskCreate(TaskFunction_t fn,
                              const char *name,
                              uint16_t stackDepth,
                              void *param,
                              UBaseType_t priority,
                              TaskHandle_t *outHandle) {
    (void)name; (void)stackDepth; (void)priority;
    std::thread *t = new std::thread(fn, param);
    if (outHandle) { *outHandle = t; }
    return pdTRUE;
}

inline void vTaskDelay(TickType_t ticks) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ticks));
}

inline void vTaskStartScheduler() {}

inline void vTaskDelete(TaskHandle_t handle) {
    if (handle) {
        if (handle->joinable()) {
            handle->join();
        }
        delete handle;
    }
}
