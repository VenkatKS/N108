/*
 * OS_Priority.c
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../MemoryManager/OS_Allocation.h"
#include "../OS.h"
#include "OS_LinkedList/OS_LinkedList.h"
#include "OS_Priority.h"
#include "../../Venkatware/venkatlib.h"
#include "../../inc/tm4c123gh6pm.h"

#include "../MemoryManager/heap.h"

ThreadQueue* PriorityBuckets[NUM_PRIORITIES];


void OS_PriorityInit(void)
{
	uint16_t priIterator = 0;
	for (priIterator = 0; priIterator < NUM_PRIORITIES; priIterator++)
	{
		PriorityBuckets[priIterator] = OS_LinkedList_NewLinkedList();
		ThreadQueue* currentBucket = PriorityBuckets[priIterator];

		if (currentBucket == 0)	// Memory allocation failed
		{
			// Kernel Panic here
			OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_PriorityInit");
		}

		currentBucket->pTCB_Head = 0;
		currentBucket->pTCB_Tail = 0;
	}
}

bool OS_AddThreadToScheduler(TCB* pTCB_NewThread)
{
	uint16_t 		CurrentThreadPriority 	= pTCB_NewThread->pri;
	ThreadQueue* 	AssociatedQueue   		= PriorityBuckets[CurrentThreadPriority];

	if (AssociatedQueue == 0) OS_KernelPanic(SCHEDULER_ILLEGALSTATE, "OS_AddThreadToScheduler");

	// Add to the end of the queue
	OS_LinkedList_InsertNodeAtEnd(AssociatedQueue, pTCB_NewThread);

	return TRUE;
}

bool OS_RemoveThreadFromScheduler(TCB* pTCB_ThreadToRemove)
{
	if (Heap_Test() == 1) OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_Sleep");
	uint16_t 		CurrentThreadPriority 	= pTCB_ThreadToRemove->pri;

	ThreadQueue* 	AssociatedQueue   		= PriorityBuckets[CurrentThreadPriority];

	AssociatedQueue->ActiveNode = 0;

	if (Heap_Test() == 1) OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_Sleep");
	OS_LinkedList_RemoveNode(AssociatedQueue, pTCB_ThreadToRemove);
	if (Heap_Test() == 1) OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_Sleep");

	return TRUE;
}

uint16_t OS_GetHighestPriority()
{
	uint16_t PriorityIterator = 0;
	for(PriorityIterator = 0; PriorityIterator < NUM_PRIORITIES; PriorityIterator++)
	{
		if (PriorityBuckets[PriorityIterator]->NumberOfNodes > 0) return PriorityIterator;
	}

	OS_KernelPanic(SCHEDULER_OUTOFTHREADS, "OS_GetHighestPriority");

	return 0; // never reaches this
}

// Return the TCB to run next
TCB* OS_GetNextThreadToRun()
{
	uint16_t NextPriority = OS_GetHighestPriority();

	ThreadQueue* NextQueue = PriorityBuckets[NextPriority];

	TCB* ThreadToRun = NextQueue->pTCB_Head;

	if (NextPriority < _OS_GetOperatingSystemInformation()->PriorityLevel)
	{
		NextQueue->ActiveNode = (NextQueue->ActiveNode + 1) % NextQueue->NumberOfNodes;

		uint32_t Iterator = 0;
		for (Iterator = 0; Iterator <= NextQueue->ActiveNode; Iterator++)
			ThreadToRun = ThreadToRun->pTCB_nextThreadBlock;

		if (ThreadToRun == 0) OS_KernelPanic(LINKEDLIST_ILLEGALCONNECTIONS, "OS_GetNextThreadToRun");

	}
	else
	{
		NextQueue->ActiveNode = (NextQueue->ActiveNode + 1) % NextQueue->NumberOfNodes;

		uint32_t Iterator = 0;
		for (Iterator = 0; Iterator <= NextQueue->ActiveNode; Iterator++)
			ThreadToRun = ThreadToRun->pTCB_nextThreadBlock;
	}

	return ThreadToRun;


}

