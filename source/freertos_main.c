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

/*
 * These are prototypes for tasks.
 */
static void hello_task(void *pvParameters);

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

    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL) !=
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
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
    for (;;)
    {
    	TimerReset();

    	/* Do stuff */
        vTaskDelay(25 / portTICK_PERIOD_MS);

        TimerLog("hello_task");
    }
}
