/*
 * OS_Allocation.c
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "heap.h"

#define NUM_TO_TRACK 200

uint64_t numBytesAllocated = 0;
uint32_t numItemsAllocated = 0;

uint32_t PC_DUMP[200];				// PC Address of calls to OS_NewAllocation

void  OS_AllocationInit()
{
	numBytesAllocated = 0;
	Heap_Init();
}
void* OS_NewAllocation(uint64_t numBytes)
{
	void* newMemory = Heap_Calloc(numBytes);
	if (newMemory == 0)
	{
		return 0;
	}

	numItemsAllocated++;
	numBytesAllocated += numBytes;

	return newMemory;
}


void OS_DestroyAllocation(void* allocation, uint64_t numBytes)
{
	Heap_Free(allocation);
	numBytesAllocated--;
}

void OS_DestroyAllocationNonTracked(void* allocation)
{
	Heap_Free(allocation);
}


