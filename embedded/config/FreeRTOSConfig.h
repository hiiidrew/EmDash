// Copyright 2024 EmDash
// SPDX-License-Identifier: MIT

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*
 * FreeRTOS kernel configuration for the EmDash simulated platform.
 * These settings are deliberately conservative to keep the example
 * portable across host based builds and small RISC-V targets.
 */

#include <assert.h>

/*-----------------------------------------------------------
 * Kernel behaviour and scheduler configuration
 *----------------------------------------------------------*/
#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configCPU_CLOCK_HZ              ( 100000000UL )
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 16 * 1024 ) )
#define configMAX_TASK_NAME_LEN         16
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         1

/*-----------------------------------------------------------
 * Synchronisation primitives
 *----------------------------------------------------------*/
#define configUSE_MUTEXES               1
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_COUNTING_SEMAPHORES   1
#define configQUEUE_REGISTRY_SIZE       8

/*-----------------------------------------------------------
 * Software timer definitions
 *----------------------------------------------------------*/
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE * 2 )

/*-----------------------------------------------------------
 * Memory allocation
 *----------------------------------------------------------*/
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  0
#define configUSE_MALLOC_FAILED_HOOK    1

/*-----------------------------------------------------------
 * Debugging aids
 *----------------------------------------------------------*/
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configASSERT(x)                assert(x)
#define configGENERATE_RUN_TIME_STATS  0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE() 0

/*-----------------------------------------------------------
 * API inclusion options
 *----------------------------------------------------------*/
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskResume                 1
#define INCLUDE_vTaskGetSchedulerState      1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetIdleTaskHandle      1
#define INCLUDE_eTaskGetState               1

#endif /* FREERTOS_CONFIG_H */

