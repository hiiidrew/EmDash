// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
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

/**
 * Basic telemetry message container. When nlohmann/json is available the
 * payload is structured JSON, otherwise a plain string is used to keep the
 * example self contained.
 */
struct TelemetryMessage {
#if HAS_JSON
    nlohmann::json payload;
#else
    std::string payload;
#endif
};

/**
 * Thread safe queue used to hand telemetry messages from a producer task to
 * any number of consumers. The implementation is intentionally lightweight to
 * remain suitable for unit testing and host based simulation.
 */
class TelemetryQueue {
public:
    void push(TelemetryMessage msg);
    bool pop(TelemetryMessage &msg);
    void close();

private:
    std::queue<TelemetryMessage> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool finished_ = false;
};

/**
 * Simple task that periodically emits telemetry messages into a provided
 * queue. It runs in its own std::thread and can be stopped via the stop()
 * method which joins the underlying thread.
 */
class TelemetryTask {
public:
    explicit TelemetryTask(TelemetryQueue &queue);
    ~TelemetryTask();

    void start();
    void stop();

private:
    void run();

    TelemetryQueue &queue_;
    std::thread thread_;
    std::atomic<bool> running_{false};
};

