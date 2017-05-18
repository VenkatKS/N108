/*
 * Scheduler.h
 *
 *  Created on: Feb 3, 2016
 *      Author: Venkat
 */

#ifndef OS_CRITICAL_SCHEDULER_H_
#define OS_CRITICAL_SCHEDULER_H_

#define MAX_THREADS 10

#define TIME_SPLICE_RESOLUTION 20000  // 20 units = 1 microsecond, resolution is 1 ms right now

#define DEFAULT_PORTF_PRI 3

bool OS_AddPeriodicThread(void(*timerTask)(void), uint32_t interruptPeriodMS, uint32_t pri);
void OS_AddSW1Task(void(*taskToExecute)(void), uint64_t priority);
void OS_AddSW2Task(void(*taskToExecute)(void), uint64_t priority);




#endif /* OS_CRITICAL_SCHEDULER_H_ */
