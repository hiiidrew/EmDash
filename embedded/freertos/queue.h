#pragma once

#include "FreeRTOS.h"
#include <mutex>
#include <queue>
#include <vector>
#include <cstring>
#include <condition_variable>
#include <chrono>

struct QueueStub {
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::vector<uint8_t>> q;
    size_t item_size;
};

using QueueHandle_t = QueueStub*;

inline QueueHandle_t xQueueCreate(UBaseType_t length, UBaseType_t itemSize) {
    (void)length;
    return new QueueStub{std::mutex(), std::condition_variable(), std::queue<std::vector<uint8_t>>(), itemSize};
}

inline BaseType_t xQueueSend(QueueHandle_t q, const void *item, TickType_t ticks) {
    (void)ticks;
    std::lock_guard<std::mutex> lock(q->mtx);
    std::vector<uint8_t> buf(q->item_size);
    std::memcpy(buf.data(), item, q->item_size);
    q->q.push(std::move(buf));
    q->cv.notify_one();
    return pdTRUE;
}

inline BaseType_t xQueueReceive(QueueHandle_t q, void *out, TickType_t ticks) {
    std::unique_lock<std::mutex> lock(q->mtx);
    if (ticks == portMAX_DELAY) {
        q->cv.wait(lock, [q]{ return !q->q.empty(); });
    } else {
        if (!q->cv.wait_for(lock, std::chrono::milliseconds(ticks), [q]{ return !q->q.empty(); })) {
            return pdFALSE;
        }
    }
    auto buf = std::move(q->q.front());
    q->q.pop();
    std::memcpy(out, buf.data(), q->item_size);
    return pdTRUE;
}
