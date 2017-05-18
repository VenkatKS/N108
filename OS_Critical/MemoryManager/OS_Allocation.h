/*
 * OS_Allocation.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_MEMORYMANAGER_OS_ALLOCATION_H_
#define SILK_OS_CRITICAL_MEMORYMANAGER_OS_ALLOCATION_H_

void  OS_AllocationInit();
void* OS_NewAllocation(uint64_t numBytes);
void OS_DestroyAllocation(void* allocation, uint64_t numBytes);
void OS_DestroyAllocationNonTracked(void* allocation);



#endif /* SILK_OS_CRITICAL_MEMORYMANAGER_OS_ALLOCATION_H_ */
