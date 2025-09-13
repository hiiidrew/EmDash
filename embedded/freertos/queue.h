#pragma once

#include "FreeRTOS.h"
#include <mutex>
#include <queue>
#include <vector>
#include <cstring>

struct QueueStub {
    std::mutex mtx;
    std::queue<std::vector<uint8_t>> q;
    size_t item_size;
};

using QueueHandle_t = QueueStub*;

inline QueueHandle_t xQueueCreate(UBaseType_t length, UBaseType_t itemSize) {
    (void)length;
    return new QueueStub{std::mutex(), std::queue<std::vector<uint8_t>>(), itemSize};
}

inline BaseType_t xQueueSend(QueueHandle_t q, const void *item, TickType_t ticks) {
    (void)ticks;
    std::lock_guard<std::mutex> lock(q->mtx);
    std::vector<uint8_t> buf(q->item_size);
    std::memcpy(buf.data(), item, q->item_size);
    q->q.push(std::move(buf));
    return pdTRUE;
}

inline BaseType_t xQueueReceive(QueueHandle_t q, void *out, TickType_t ticks) {
    (void)ticks;
    std::lock_guard<std::mutex> lock(q->mtx);
    if (q->q.empty()) {
        return pdFALSE;
    }
    auto buf = std::move(q->q.front());
    q->q.pop();
    std::memcpy(out, buf.data(), q->item_size);
    return pdTRUE;
}
