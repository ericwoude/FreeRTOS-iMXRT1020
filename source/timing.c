/*
 * timing.c
 *
 *  Created on: May 20, 2020
 *      Author: Eric van der Woude
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"

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

#define queue_size 100

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ConfigureRunTimeStatsTimer();
void GPT2_IRQHandler();
void TimerReset();
void TimerLog(const char* name);
void TimerPrint();
void QueueInit();
void vApplicationIdleHook();

/*******************************************************************************
 * Globals
 ******************************************************************************/
static QueueHandle_t xInfoQueue = NULL;

/*
 * The max bound is 4,294,967,295, which means periods shorter than
 * 4294967295 / (1 / (500 * 10^-6)) = 2147483 seconds can be counted
 */
unsigned long RunTimeCounter;

/*
 * The data structure contains the task's name and the run time in milliseconds.
 */
typedef struct
{
	const char* name;
	unsigned long time;
} task_info;

void QueueInit()
{
	xInfoQueue = xQueueCreate(queue_size, sizeof(task_info));

    /* Enable queue view in MCUX IDE FreeRTOS TAD plugin. */
    if (xInfoQueue != NULL)
    {
        vQueueAddToRegistry(xInfoQueue, "LogQ");
    }
}

/*!
 * @brief Timer responsible for keeping track of run time statistics.
 */
void ConfigureRunTimeStatsTimer()
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
void TimerReset()
{
	RunTimeCounter = 0;
}

/*!
 * @brief Logs the info of the task and sends it to a queue.
 */
void TimerLog(const char* name)
{
	task_info* info = malloc(sizeof(task_info));
	if (info)
	{
		info->name = name;
		info->time = COUNT_TO_MSEC(RunTimeCounter, gptTICK_RATE_HZ);

		xQueueSend(xInfoQueue, (void *) &info, ( TickType_t ) 0);
	}
	else
	{
		PRINTF("Error creating task_info struct");
	}
}

/*!
 * @brief Tries to receive a task info from queue and prints the content.
 */
void TimerPrint()
{
	task_info* info;

	if (xInfoQueue != NULL)
	{
		for (;;)
		{
			/* Performs a non-blocking receive. */
			if (xQueueReceive(xInfoQueue, &info, ( TickType_t ) 0 ) == pdPASS)
			{
				PRINTF("%s %u\n", info->name, info->time);
			}
		}
	}
}

void vApplicationIdleHook()
{
	TimerPrint();
}
