#pragma once

#include "task.hpp"
#include "telemetry_task.hpp"
#include <iostream>

/**
 * Task consuming telemetry messages and performing AI processing.
 * Demonstrates inheritance and polymorphic behaviour.
 */
class AITask : public Task {
public:
    AITask();
protected:
    void run() override;
};

