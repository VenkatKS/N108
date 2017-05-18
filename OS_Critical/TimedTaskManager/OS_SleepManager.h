/*
 * OS_SleepManager.h
 *
 *  Created on: Mar 1, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_TIMEDTASKMANAGER_OS_SLEEPMANAGER_H_
#define SILK_OS_CRITICAL_TIMEDTASKMANAGER_OS_SLEEPMANAGER_H_

#ifndef OS_CRITICAL_OS_H_
#include "../OS.h"
#endif

void OS_SleepManagerInit(void);
void OS_NewSleepingThread(TCB* pTCB_SleepingThread);





// Should not be called by any part of the kernel except for the systemtimer
void _INTERNAL_OS_SleepManager(void);



#endif /* SILK_OS_CRITICAL_TIMEDTASKMANAGER_OS_SLEEPMANAGER_H_ */
