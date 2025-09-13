#include <cassert>
#include <chrono>
#include <thread>

#include "../rtos/tasks/scheduler.hpp"
#include "../rtos/tasks/task.hpp"
#include <FreeRTOS.h>
#include <task.h>

class DummyTask : public Task {
public:
    DummyTask() : Task("dummy", tskIDLE_PRIORITY, configMINIMAL_STACK_SIZE), counter(0) {}
    int counter;
protected:
    void run() override {
        ++counter;
        vTaskDelete(nullptr);
    }
};

int main() {
    Scheduler sched;
    DummyTask task;
    sched.add_task(&task);
    sched.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    sched.stop();
    assert(task.counter == 1);
    return 0;
}
