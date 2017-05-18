/*
 * Scheduler.c
 *
 *  Created on: Feb 3, 2016
 *      Author: Venkat
 */





#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Timer2Driver/Timer2.h"
#include "Timer3Driver/Timer3.h"
#include "PortFDriver/PortF_Edge.h"
#include "../../Venkatware/venkatlib.h"
#include "PortFDriver/PortF_Edge.h"
#include "OS_BackgroundThread.h"

unsigned int ThreadCount = 0;

uint8_t NumberOfPeriodicThreads = 0;

bool PeriodicThread1InUse = FALSE;
bool PeriodicThread2InUse = FALSE;

bool OS_AddPeriodicThread(void(*timerTask)(void), uint32_t interruptPeriodMS, uint32_t pri)
{
	if (NumberOfPeriodicThreads == 2) return FALSE;
	if (!PeriodicThread1InUse)
	{
		OverflowManager_Init(timerTask, interruptPeriodMS, pri);
		PeriodicThread1InUse = TRUE;
	}
	else if (!PeriodicThread2InUse)
	{
		Timer3_Init(timerTask, interruptPeriodMS, pri);
		PeriodicThread2InUse = TRUE;
	}

	return TRUE;
}


void OS_AddSW1Task(void(*taskToExecute)(void), uint64_t priority)
{
	uint32_t Start = StartCritical();
	if (PortFEdge_GetInitState() == FALSE) PortFEdge_Init(priority);
	PortFEdge_SetFunction1(taskToExecute);
	EndCritical(Start);
}

void OS_AddSW2Task(void(*taskToExecute)(void), uint64_t priority)
{
	uint32_t Start = StartCritical();
	if (PortFEdge_GetInitState() == FALSE) PortFEdge_Init(priority);
	PortFEdge_SetFunction2(taskToExecute);
	EndCritical(Start);
}

void _OS_InitializePortF()
{
	if (PortFEdge_GetInitState() == FALSE) PortFEdge_Init(DEFAULT_PORTF_PRI);
}

