/*
 * tasksets.c
 *
 *  Created on: May 23, 2020
 *      Author: riccus
 */


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


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern unsigned long TimerCount();
extern void TimerLog(const char* name, unsigned long absRunTimeCounter);
extern void ComputationTime(unsigned long msec);

void vTaskA1(void *pvParameters);
void vTaskA2(void *pvParameters);
void vTaskA3(void *pvParameters);


/*******************************************************************************
 * Code
 ******************************************************************************/
void vTaskA1(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();

	vTaskSetApplicationTaskTag(NULL, ( void * ) 1);

	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		unsigned long startCount = TimerCount();

		/* Do stuff for given duration in milliseconds */
		ComputationTime(200);

		TimerLog("Task 1", TimerCount() - startCount);
	}
}
void vTaskA2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();

	vTaskSetApplicationTaskTag(NULL, ( void * ) 2);

	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		unsigned long startCount = TimerCount();

		/* Do stuff for given duration in milliseconds */
		ComputationTime(300);

		TimerLog("Task 2", TimerCount() - startCount);
	}
}
/*!
 * @brief Task responsible for printing of "Task 1" message.
 */
//void vTaskA1(void *pvParameters)
//{
//	TickType_t xLastWakeTime;
//	const TickType_t xFrequency = 150 / portTICK_PERIOD_MS;
//	xLastWakeTime = xTaskGetTickCount();
//
//	for (;;)
//	{
//		vTaskDelayUntil(&xLastWakeTime, xFrequency);
//		unsigned long startCount = TimerCount();
//
//		/* Do stuff for given duration in milliseconds */
//		ComputationTime(50);
//
//		TimerLog("Task 1", TimerCount() - startCount);
//	}
//}

/*!
 * @brief Task responsible for printing of "Task 2" message.
 */
//void vTaskA2(void *pvParameters)
//{
//	TickType_t xLastWakeTime;
//	const TickType_t xFrequency = 300 / portTICK_PERIOD_MS;
//	xLastWakeTime = xTaskGetTickCount();
//
//	for (;;)
//	{
//		vTaskDelayUntil(&xLastWakeTime, xFrequency);
//		TimerReset();
//
//		/* Do stuff for given duration in milliseconds */
//		ComputationTime(75);
//
//		TimerLog("Task 2");
//	}
//}
//
///*!
// * @brief Task responsible for printing of "Task 3" message.
// */
//void vTaskA3(void *pvParameters)
//{
//	TickType_t xLastWakeTime;
//	const TickType_t xFrequency = 20 / portTICK_PERIOD_MS;
//	xLastWakeTime = xTaskGetTickCount();
//
//	for (;;)
//	{
//		vTaskDelayUntil(&xLastWakeTime, xFrequency);
//		TimerReset();
//
//		/* Do stuff for given duration in milliseconds */
//		ComputationTime(5);
//
//		TimerLog("Task 3");
//	}
//}
