#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "rtos/tasks/telemetry_task.hpp"

/**
 * Simple emulation of an embedded flight system.  The goal of this
 * file is to provide a stand‑alone example that can later be expanded
 * to make use of FreeRTOS, QEMU, Qt and other technologies listed in the
 * project roadmap.  For now it demonstrates a minimal producer/consumer
 * model for telemetry data and a placeholder for AI assisted processing.
 */

using namespace std::chrono_literals;

static void ai_task(TelemetryQueue &q) {
    TelemetryMessage msg;
    while (q.pop(msg)) {
#if HAS_JSON
        std::cout << "AI received telemetry: " << msg.payload.dump() << '\n';
#else
        std::cout << "AI received telemetry: " << msg.payload << '\n';
#endif
        std::this_thread::sleep_for(150ms);
    }
    std::cout << "AI task exiting" << std::endl;
}

int main() {
    std::cout << "Starting EmDash Embedded Simulation" << std::endl;
    TelemetryQueue queue;
    TelemetryTask producer(queue);
    producer.start();

    std::thread consumer(ai_task, std::ref(queue));

    consumer.join();
    producer.stop();

    std::cout << "System shutdown" << std::endl;
    return 0;
}

