#pragma once

#include <FreeRTOS.h>
#include <task.h>

/**
 * Abstract base class representing a FreeRTOS task.  Derived classes
 * implement the run() method which is invoked inside the task context.
 */
class Task {
public:
    Task(const char *name, UBaseType_t priority, uint16_t stackDepth)
        : name_(name), priority_(priority), stackDepth_(stackDepth), handle_(nullptr) {}
    virtual ~Task() = default;

    /** Create the underlying FreeRTOS task */
    void start() { xTaskCreate(entry, name_, stackDepth_, this, priority_, &handle_); }

    /** Return the task handle for management operations */
    TaskHandle_t handle() const { return handle_; }

protected:
    /** Function executed within the task context */
    virtual void run() = 0;

private:
    static void entry(void *param) {
        static_cast<Task *>(param)->run();
    }

    const char *name_;
    UBaseType_t priority_;
    uint16_t stackDepth_;
    TaskHandle_t handle_;
};

