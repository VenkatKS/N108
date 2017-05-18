/*
 * LinkedList.c
 *
 *  Created on: Feb 19, 2016
 *      Author: Venkat
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../OS.h"
#include "OS_LinkedList.h"
#include "../../MemoryManager/OS_Allocation.h"

ThreadQueue* OS_LinkedList_NewLinkedList()
{
	ThreadQueue* pQ_NewQ = OS_NewAllocation(sizeof(ThreadQueue) * 1); // Allocate space for one TCB

	if (pQ_NewQ == 0)
	{
		OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_LinkedList_NewLinkedList");
	}

	pQ_NewQ->pTCB_Head 		= 0;
	pQ_NewQ->pTCB_Tail 		= 0;
	pQ_NewQ->NumberOfNodes	= 0;
	//OS_Semaphore_bInit(&pQ_NewQ->ListLock);
	return pQ_NewQ;
}

// WARNING: Requires OS_Wait call for semaphore. Cannot be called when Interrupts are disabled.
void OS_LinkedList_InsertNodeAtEnd(ThreadQueue* pQ_Host, TCB* pTCB_NodeToInsert)
{
	//OS_Semaphore_bWait(&pQ_Host->ListLock);

	TCB* pTCB_CurrentThreadBeingAdded = 0;

	if (pQ_Host->NumberOfNodes == 0)
	{
		pTCB_CurrentThreadBeingAdded = pTCB_NodeToInsert;

		pQ_Host->pTCB_Tail = pTCB_CurrentThreadBeingAdded;

		// First thread being added, so don't worry about anything here
		pTCB_CurrentThreadBeingAdded->pTCB_nextThreadBlock = pTCB_CurrentThreadBeingAdded;
		pTCB_CurrentThreadBeingAdded->pTCB_prevThreadBlock = pTCB_CurrentThreadBeingAdded;

		pQ_Host->NumberOfNodes++;

		pQ_Host->pTCB_Head = pTCB_CurrentThreadBeingAdded;
	}
	else
	{
		pTCB_CurrentThreadBeingAdded = pTCB_NodeToInsert; // Allocate space for one TCB

		pTCB_CurrentThreadBeingAdded->pTCB_prevThreadBlock = pQ_Host->pTCB_Tail;
		pQ_Host->pTCB_Tail->pTCB_nextThreadBlock = pTCB_CurrentThreadBeingAdded;
		pQ_Host->pTCB_Tail = pTCB_CurrentThreadBeingAdded;
		pTCB_CurrentThreadBeingAdded->pTCB_nextThreadBlock = pQ_Host->pTCB_Head;

		pQ_Host->NumberOfNodes++;
	}

	//OS_Semaphore_bSignal(&pQ_Host->ListLock);

}

void OS_LinkedList_RemoveNode(ThreadQueue* pQ_Host, TCB* pTCB_NodeToDelete)
{
	//OS_Semaphore_bWait(&pQ_Host->ListLock);

	TCB* currentThread = pTCB_NodeToDelete;

	// First Thread in Linked List
	if (currentThread == pQ_Host->pTCB_Head)
	{
		TCB* nextThread  = currentThread->pTCB_nextThreadBlock;
		TCB* lastThread  = pQ_Host->pTCB_Tail;

		nextThread->pTCB_prevThreadBlock = 0;
		lastThread->pTCB_nextThreadBlock = nextThread;

		pQ_Host->pTCB_Head = nextThread;


		if (pQ_Host->NumberOfNodes > 1) pQ_Host->pTCB_Head = nextThread;
		else
		{
			pQ_Host->pTCB_Head = 0;
			pQ_Host->pTCB_Tail = 0;
		}

	}
	else if (currentThread == pQ_Host->pTCB_Tail) // last thread in linked list
	{
		TCB* prevThread		= currentThread->pTCB_prevThreadBlock;
		TCB* firstThread 	= pQ_Host->pTCB_Head;

		prevThread->pTCB_nextThreadBlock = firstThread;

		pQ_Host->pTCB_Tail = prevThread;
	}
	else
	{
		TCB* nextThread  = currentThread->pTCB_nextThreadBlock;
		TCB* prevThread	 = currentThread->pTCB_prevThreadBlock;

		if (prevThread == 0 || nextThread == 0) OS_KernelPanic(ACTIVELINKEDLIST_ILLEGALCONNECTIONS, "OS_LinkedList_RemoveNode");

		prevThread->pTCB_nextThreadBlock = nextThread;
		nextThread->pTCB_prevThreadBlock = prevThread;

		//currentThread->pTCB_prevThreadBlock = 0;
	}
	pQ_Host->NumberOfNodes--;
	//OS_Semaphore_bSignal(&pQ_Host->ListLock);
}

