#include <vector>
#include <thread>
#include <functional>
#include <chrono>
#include <atomic>
#include <string>

/**
 * Minimal cooperative scheduler used for unit testing and host based
 * simulation.  It is not a full RTOS replacement but provides an
 * abstraction that mimics periodic task execution.  Tasks are executed
 * in their own std::thread and repeat at the user supplied period until
 * the scheduler is stopped.
 */
class Scheduler {
public:
    using TaskFn = std::function<void()>;

    struct Task {
        std::string name;                       ///< Friendly task name
        TaskFn      fn;                         ///< Function to run
        std::chrono::milliseconds period{0};    ///< Period between runs
    };

    ~Scheduler() { stop(); }

    void add_task(Task task) {
        tasks_.push_back(std::move(task));
    }

    void start() {
        if (running_) {
            return; // already running
        }
        running_ = true;
        for (auto &task : tasks_) {
            threads_.emplace_back([this, task]() {
                while (running_) {
                    auto start_time = std::chrono::steady_clock::now();
                    task.fn();
                    auto end_time = std::chrono::steady_clock::now();
                    auto elapsed =
                        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                    if (elapsed < task.period) {
                        std::this_thread::sleep_for(task.period - elapsed);
                    }
                }
            });
        }
    }

    void stop() {
        if (!running_) {
            return;
        }
        running_ = false;
        for (auto &t : threads_) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads_.clear();
    }

private:
    std::vector<Task> tasks_;
    std::vector<std::thread> threads_;
    std::atomic<bool> running_{false};
};

// Example usage stub demonstrating how the scheduler might be wired
// into the broader system.  This is not executed in production builds
// but serves as a reference.
#ifdef SCHEDULER_DEMO
#include <iostream>
int main() {
    Scheduler sched;
    sched.add_task({"heartbeat", [](){ std::cout << "tick\n"; }, std::chrono::milliseconds(100)});
    sched.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sched.stop();
    return 0;
}
#endif

