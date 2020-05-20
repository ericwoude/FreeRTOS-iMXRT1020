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
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
// The tick frequency of the general purpose timer is 10 times the FreeRTOS tick.
#define gptTICK_RATE_HZ (10 * (unsigned int) configTICK_RATE_HZ)
#define gptTICK_RATE_US (1000000 / gptTICK_RATE_HZ)


/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ConfigureRunTimeStatsTimer();
void GPT2_IRQHandler();
static void TimerReset();
static void TimerPrint();

static void hello_task(void *pvParameters);



/*******************************************************************************
 * Globals
 ******************************************************************************/
static unsigned long RunTimeCounter;
// max bound 4,294,967,295
// which means periods shorter than 4294967295 / (1 / (500 * 10^-6)) = 2147483 seconds can be counted
// every second the counter should be 2000 higher

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
        vTaskDelay(50 / portTICK_PERIOD_MS);

        TimerPrint("hello_task");
    }
}

/*!
 * @brief Timer responsible for keeping track of run time statistics.
 */
static void ConfigureRunTimeStatsTimer()
{
	unsigned long gptFreq;
	gpt_config_t gptConfig;

	GPT_GetDefaultConfig(&gptConfig);

	/* Initialize GPT module */
	GPT_Init(GPT2, &gptConfig);

	/* Divide GPT clock source frequency by 3 inside GPT module */
	GPT_SetClockDivider(GPT2, 3);

	/* Get GPT clock frequency */
	gptFreq = CLOCK_GetFreq(kCLOCK_PerClk);

	/* GPT frequency is divided by 3 inside module */
	gptFreq /= 3;

	/* By setting the tick period to 500us (2 kHz), the rate would
	 * be 10x higher than the FreeRTOS tick, giving more accurate results. */
	gptFreq = USEC_TO_COUNT(gptTICK_RATE_US, gptFreq);
	GPT_SetOutputCompareValue(GPT2, kGPT_OutputCompare_Channel1, gptFreq);

	/* Enable GPT Output Compare1 interrupt */
	GPT_EnableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);

	/* Enable at the Interrupt and start timer */
	EnableIRQ(GPT2_IRQn);
	GPT_StartTimer(GPT2);
}

/*!
 * @brief This IRQ handler will increment the run time counter every tick.
 */
void GPT2_IRQHandler() {
  /* Clear interrupt flag.*/
  GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
  RunTimeCounter++; /* increment runtime counter */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
  __DSB();
#endif
}

/*!
 * @brief Resets run time counter to 0.
 */
static void TimerReset()
{
	RunTimeCounter = 0;
}

/*!
 * @brief Reads the run time counter in milliseconds seconds.
 */
static void TimerPrint(const char* name)
{
	PRINTF("%s %u\n", name, COUNT_TO_MSEC(RunTimeCounter, gptTICK_RATE_HZ));
}
