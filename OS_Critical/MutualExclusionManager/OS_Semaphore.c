/*
 * OS_Semaphore.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Venkat
 */

#include <stdint.h>
#include "../OS.h"
#include "../PriorityManager/OS_Priority.h"
#include "OS_Semaphore.h"
#include "../MemoryManager/OS_Allocation.h"

Semaphore* OS_InitSemaphore(uint32_t numPermits)
{
	Semaphore* newSemaphore = OS_NewAllocation(sizeof(Semaphore) * 1);

	if (newSemaphore == 0) return 0;

	newSemaphore->permits = numPermits;

	return newSemaphore;
}

void OS_Wait(Semaphore* SemToWaitOn)	// Utilizes one permit per thread
{
	DisableInterrupts();

	while(SemToWaitOn->permits <= 0)
	{
		TCB* CurrentThread = _OS_GetOperatingSystemInformation()->pTCB_RunPt;
		OS_RemoveThreadFromScheduler(CurrentThread);
		_OS_AddThreadToSemaphore(SemToWaitOn, CurrentThread);
		EnableInterrupts();
		OS_Suspend();
		DisableInterrupts();
	}

	if (SemToWaitOn->permits <= 0) OS_KernelPanic(SEMAPHORE_ILLEGALSTATE, "OS_Wait");

	SemToWaitOn->permits = SemToWaitOn->permits - 1;

	EnableInterrupts();
}

void OS_Signal(Semaphore* SemToWaitOn)
{
	uint8_t prevState = StartCritical();
	SemToWaitOn->permits = SemToWaitOn->permits + 1;
	if (SemToWaitOn->BlockedThreads.NumberOfNodes > 0)
	{
		TCB* HighestPriorityThread = SemToWaitOn->BlockedThreads.pTCB_Head;

		TCB* ThreadIterator = SemToWaitOn->BlockedThreads.pTCB_Head;

		int i = 0;
		for (i = 0; i < SemToWaitOn->BlockedThreads.NumberOfNodes; i++)
		{
			TCB* NextThread = ThreadIterator->pTCB_nextThreadBlock;
			if (NextThread->pri < HighestPriorityThread->pri)
			{
				HighestPriorityThread = NextThread;
			}
			ThreadIterator = NextThread;
		}

		_OS_RemoveThreadFromSemaphore(SemToWaitOn, HighestPriorityThread);
		OS_AddThreadToScheduler(HighestPriorityThread);
	}

	OS_Suspend();
	EndCritical(prevState);
}

// Internal function. User threads should not call this
void _OS_AddThreadToSemaphore(Semaphore* Sem4, TCB* ThreadToAdd)
{
	OS_LinkedList_InsertNodeAtEnd(&Sem4->BlockedThreads, ThreadToAdd);
}

void _OS_RemoveThreadFromSemaphore(Semaphore* Sem4, TCB* ThreadToRemove)
{
	OS_LinkedList_RemoveNode(&Sem4->BlockedThreads, ThreadToRemove);
}
