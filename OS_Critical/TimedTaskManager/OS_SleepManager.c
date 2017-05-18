/*
 * OS_SleepManager.c
 *
 *  Created on: Mar 1, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../OS.h"
#include "Timer1Driver/SystemTimer.h"
#include "../PriorityManager/OS_LinkedList/OS_LinkedList.h"
#include "../PriorityManager/OS_Priority.h"
ThreadQueue* SleepingList;

// TODO: Add a semaphore for the sleep manager

void OS_SleepManagerInit(void)
{
	SleepingList = OS_LinkedList_NewLinkedList();
	if (SleepingList == 0)
	{
		OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_SleepManagerInit");
	}

	SystemTimer_Init(TIME_1MS);
}


void OS_NewSleepingThread(TCB* pTCB_SleepingThread)
{
	pTCB_SleepingThread->Sleep_State = ASLEEP;
	OS_LinkedList_InsertNodeAtEnd(SleepingList, pTCB_SleepingThread);
}

void OS_RemoveThreadFromSleep(TCB* ThreadToRemove)
{
	ThreadToRemove->Sleep_State = AWAKE;
	OS_LinkedList_RemoveNode(SleepingList, ThreadToRemove);
	OS_AddThreadToScheduler(ThreadToRemove);
}

void _INTERNAL_OS_SleepManager(void)  // Interrupts should be disabled in this area
{
	if (SleepingList->NumberOfNodes == 0) return;

	uint64_t ThreadIterator 			= 0;
	TCB*		 pTCB_ThreadIterator		= SleepingList->pTCB_Head;
	TCB*     pTCB_nxThread			= 0;
	for(ThreadIterator = 0; ThreadIterator < SleepingList->NumberOfNodes; ThreadIterator++)
	{
		pTCB_ThreadIterator->SleepTimeUnitsLeft--;
		pTCB_nxThread = pTCB_ThreadIterator->pTCB_nextThreadBlock;
		if (pTCB_ThreadIterator->SleepTimeUnitsLeft == 0)
		{
			OS_RemoveThreadFromSleep(pTCB_ThreadIterator);
			OS_Suspend();
			ThreadIterator--;
		}

		pTCB_ThreadIterator = pTCB_nxThread;
	}
}
