#include <FreeRTOS.h>
#include <queue.h>
#include <iostream>
#include <task.h>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

#ifdef __has_include
#  if __has_include(<nlohmann/json.hpp>)
#    include <nlohmann/json.hpp>
#    define HAS_JSON 1
#  else
#    define HAS_JSON 0
#  endif
#else
#  define HAS_JSON 0
#endif

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


/**
 * Simple emulation of an embedded flight system.  The goal of this
 * file is to provide a stand‑alone example that can later be expanded
 * to make use of FreeRTOS, QEMU, Qt and other technologies listed in the
 * project roadmap.  For now it demonstrates a minimal producer/consumer
 * model for telemetry data and a placeholder for AI assisted processing.
 */

using namespace std::chrono_literals;

struct TelemetryMessage {
#if HAS_JSON
    nlohmann::json payload;
#else
    std::string payload;
#endif
};

class TelemetryQueue {
public:
    void push(TelemetryMessage msg) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(msg));
        }
        cv_.notify_one();
    }

    bool pop(TelemetryMessage &msg) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&] { return finished_ || !queue_.empty(); });
        if (queue_.empty()) {
            return false;
        }
        msg = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            finished_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<TelemetryMessage> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool finished_ = false;
};

static void sensor_task(TelemetryQueue &q) {
    for (int i = 0; i < 5; ++i) {
        TelemetryMessage msg;
#if HAS_JSON
        msg.payload = {
            {"timestamp", i},
            {"altitude", 1000 + i * 10},
            {"velocity", 50 + i}
        };
#else
        msg.payload = "tick:" + std::to_string(i);
#endif
        q.push(std::move(msg));
        std::this_thread::sleep_for(100ms);
    }
    q.close();
}

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

    std::thread producer(sensor_task, std::ref(queue));
    std::thread consumer(ai_task, std::ref(queue));

    producer.join();
    consumer.join();

    std::cout << "System shutdown" << std::endl;
main
    return 0;
}

