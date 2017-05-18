/*
 * OS_Semaphore.h
 *
 *  Created on: Mar 3, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_MUTUALEXCLUSIONMANAGER_OS_SEMAPHORE_H_
#define SILK_OS_CRITICAL_MUTUALEXCLUSIONMANAGER_OS_SEMAPHORE_H_
#ifndef LAB3_VRTOS_OS_CRITICAL_OS_LINKEDLIST_OS_LINKEDLIST_H_
#include "../PriorityManager/OS_LinkedList/OS_LinkedList.h"
#endif

typedef struct nRTOS_Mutex
{
	uint32_t permits;

	ThreadQueue BlockedThreads;

} Semaphore;

// Semaphore functions
Semaphore* 	OS_InitSemaphore(uint32_t numPermits);
void 		OS_Wait(Semaphore* SemToWaitOn);	// Utilizes one permit per thread;
void 		OS_Signal(Semaphore* SemToWaitOn);



// Intra-Kernel Functions
void 		_OS_AddThreadToSemaphore(Semaphore* Sem4, TCB* ThreadToAdd);
void 		_OS_RemoveThreadFromSemaphore(Semaphore* Sem4, TCB* ThreadToRemove);



#endif /* SILK_OS_CRITICAL_MUTUALEXCLUSIONMANAGER_OS_SEMAPHORE_H_ */
