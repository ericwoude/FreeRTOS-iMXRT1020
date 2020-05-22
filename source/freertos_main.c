/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"


/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 * These functions are used to keep track of task info.
 */
extern void ConfigureRunTimeStatsTimer();
extern void TimerReset();
extern void TimerLog(const char* name);
extern void QueueInit();
extern void vApplicationIdleHook();
extern void ComputationTime(unsigned long msec);

/*
 * These are prototypes for tasks.
 */
static void task_1(void *pvParameters);
static void task_2(void *pvParameters);
static void task_3(void *pvParameters);

/*
 * TODO:
 * Create a data structure info, which holds a task name and unsigned integer
 * Create a queue with n of these data structure pointers
 * Create an idle task hook, while idle start emptying the queue by sending this data to the host machine
 * Create a function that empties the queue if it is full, do a check in every TimerPrint() call
 */


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    if (xTaskCreate(task_1, "task_1", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(task_2, "task_2", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(task_3, "task_3", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Start the timer. */
    QueueInit();
    ConfigureRunTimeStatsTimer();

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Task 1" message.
 */
static void task_1(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		TimerReset();

		/* Do stuff for 2000 ms */
		ComputationTime(200);

		TimerLog("Task 1");
	}
}

/*!
 * @brief Task responsible for printing of "Task 2" message.
 */
static void task_2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		TimerReset();

		/* Do stuff for 50 ms */
		ComputationTime(50);

		TimerLog("Task 2");
	}
}

/*!
 * @brief Task responsible for printing of "Task 3" message.
 */
static void task_3(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 100 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		TimerReset();

		/* Do stuff for 5 ms */
		ComputationTime(5);

		TimerLog("Task 3");
	}
}
