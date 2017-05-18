/*
 * OS_Priority.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_PRIORITYMANAGER_OS_PRIORITY_H_
#define SILK_OS_CRITICAL_PRIORITYMANAGER_OS_PRIORITY_H_

#ifndef LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_
#include "../../Venkatware/venkatlib.h"
#endif

#define NUM_PRIORITIES 7

// Initialize the priority scheduler
void 	OS_PriorityInit(void);
bool 	OS_AddThreadToScheduler(TCB* pTCB_NewThread);
bool 	OS_RemoveThreadFromScheduler(TCB* pTCB_ThreadToRemove);
uint16_t OS_GetHighestPriority();


// Get the next thread to run
TCB* OS_GetNextThreadToRun();



#endif /* SILK_OS_CRITICAL_PRIORITYMANAGER_OS_PRIORITY_H_ */
