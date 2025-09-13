#include "scheduler.hpp"
#include "task.hpp"

#include <FreeRTOS.h>
#include <task.h>

void Scheduler::add_task(Task *task) { tasks_.push_back(task); }

void Scheduler::start() {
    for (auto *t : tasks_) {
        t->start();
    }
    vTaskStartScheduler();
}

void Scheduler::stop() {
    for (auto *t : tasks_) {
        if (t->handle() != nullptr) {
            vTaskDelete(t->handle());
        }
    }
    tasks_.clear();
}

