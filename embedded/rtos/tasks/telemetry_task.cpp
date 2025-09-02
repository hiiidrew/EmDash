// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#include "telemetry_task.hpp"

#include <chrono>
#include <utility>

using namespace std::chrono_literals;

// ----- TelemetryQueue ----------------------------------------------------

void TelemetryQueue::push(TelemetryMessage msg) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(msg));
    }
    cv_.notify_one();
}

bool TelemetryQueue::pop(TelemetryMessage &msg) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&] { return finished_ || !queue_.empty(); });
    if (queue_.empty()) {
        return false;
    }
    msg = std::move(queue_.front());
    queue_.pop();
    return true;
}

void TelemetryQueue::close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        finished_ = true;
    }
    cv_.notify_all();
}

// ----- TelemetryTask -----------------------------------------------------

TelemetryTask::TelemetryTask(TelemetryQueue &queue) : queue_(queue) {}

TelemetryTask::~TelemetryTask() { stop(); }

void TelemetryTask::start() {
    if (running_) {
        return;
    }
    running_ = true;
    thread_ = std::thread(&TelemetryTask::run, this);
}

void TelemetryTask::stop() {
    if (!running_) {
        return;
    }
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void TelemetryTask::run() {
    for (int i = 0; running_ && i < 5; ++i) {
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
        queue_.push(std::move(msg));
        std::this_thread::sleep_for(100ms);
    }
    queue_.close();
}

