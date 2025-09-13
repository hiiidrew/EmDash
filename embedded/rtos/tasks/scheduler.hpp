#pragma once

#include <vector>

class Task;

/**
 * Lightweight wrapper around the FreeRTOS scheduler to manage Task objects.
 */
class Scheduler {
public:
    void add_task(Task *task);
    void start();
    void stop();

private:
    std::vector<Task *> tasks_;
};

