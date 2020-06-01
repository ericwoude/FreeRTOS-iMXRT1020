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
//#define taskA1_PRIORITY (configMAX_PRIORITIES - 1)
//#define taskA2_PRIORITY (configMAX_PRIORITIES - 2)
//#define taskA3_PRIORITY (configMAX_PRIORITIES - 3)

#define taskA1_PRIORITY (configMAX_PRIORITIES - 2)
#define taskA2_PRIORITY (configMAX_PRIORITIES - 3)
#define taskA3_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void ConfigureRunTimeStatsTimer();
extern void QueueInit();
extern void vApplicationIdleHook();

/*
 * These are prototypes for tasks.
 */
extern void vTaskA1(void *pvParameter);
extern void vTaskA2(void *pvParameter);
extern void vTaskA3(void *pvParameter);

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

    if (xTaskCreate(vTaskA1, "TaskA1", configMINIMAL_STACK_SIZE + 100, NULL, taskA1_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

//    if (xTaskCreate(vTaskA2, "TaskA2", configMINIMAL_STACK_SIZE + 100, NULL, taskA2_PRIORITY, NULL) !=
//        pdPASS)
//    {
//        PRINTF("Task creation failed!.\r\n");
//        while (1)
//            ;
//    }
//
//    if (xTaskCreate(vTaskA3, "TaskA3", configMINIMAL_STACK_SIZE + 100, NULL, taskA3_PRIORITY, NULL) !=
//        pdPASS)
//    {
//        PRINTF("Task creation failed!.\r\n");
//        while (1)
//            ;
//    }

    /* Start the timer. */
    QueueInit();
    ConfigureRunTimeStatsTimer();

    vTaskStartScheduler();

    for (;;)
        ;
}
