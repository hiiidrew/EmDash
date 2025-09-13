#pragma once

#include <cstdint>
#include "../config/FreeRTOSConfig.h"

using BaseType_t = int;
using UBaseType_t = unsigned int;
using TickType_t = uint32_t;

#define pdMS_TO_TICKS(x) (x)
#define portMAX_DELAY 0xffffffff
#define pdTRUE 1
#define pdFALSE 0
#define tskIDLE_PRIORITY 0
