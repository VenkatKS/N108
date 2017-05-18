/*
 * OS.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_OS_H_
#define SILK_OS_CRITICAL_OS_H_

#ifndef LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_
#include "../Venkatware/venkatlib.h"
#endif

#ifndef LOADER_H_
#include "ProcessLoaderManager/loader.h"
#endif

#define STACK_WORD_SIZE 	500  // 400 byte stacks

#define TIME_1MS    		50000
#define TIME_2MS    		(2*TIME_1MS)
#define TIME_500US  		(TIME_1MS/2)
#define TIME_250US  		(TIME_1MS/5)


#define LOWPRIORITYSTATE 0xFC;


typedef enum VRTOS_SleepState
{
	AWAKE,
	ASLEEP
} SleepState;

// Offset Modes
#define REGULAR 0
#define BACKUP  4


// Operating System structs
typedef struct nRTOS_Process
{
	void*			CodeSegment;				// Pointer to the processes' code segment
	void* 			DataSegment;				// Pointer to the processes' data segment (need to load this into R9)
	unsigned short 	Proc_Priority;			// Processes' priority
	unsigned long  	pid;						// Process ID
	unsigned long  	numThreads;				// Number of threads this process owns

	void(*ProgramCounter)(void);		// The Starting Point for the Process
} PCB;



typedef struct nRTOS_Thread
{
	int* 					pInt_CurrentSP;
	struct nRTOS_Thread* 	pTCB_nextThreadBlock;
	struct nRTOS_Thread* 	pTCB_prevThreadBlock;
	struct nRTOS_Thread*		pTCB_nextBackUp;

	unsigned long			tid;

	unsigned short 			pri;

	SleepState    			Sleep_State;
	unsigned char 			Blocked_State;

	unsigned long			SleepTimeUnitsLeft;

	int 						Thread_Stack[STACK_WORD_SIZE];

	struct nRTOS_Process*   	pPCB_ParentProcess;				// If 0: OS Task, else it's a user process




} TCB;

// OS Start and Initialization routines
void 		OS_Init(void);
void 		OS_Launch(uint32_t timePeriod);

// Process Creation Functions
int OS_AddProcess(void(*entry)(void), void *text, void *data, unsigned long stackSize, unsigned long priority);




// Thread Control Functions
void 		OS_Sleep(uint32_t SleepTimeInMillis);
void 		OS_Kill(void);
void 		OS_Suspend(void);

// Thread Creation Functions
void 		OS_SetUpInitialStack(TCB* currentThread);
int 			OS_AddThread(void (*threadTask)(void), uint32_t stackSize, uint32_t priority);

// Thread Information Functions
uint32_t 	OS_Id(void);
uint32_t 	OS_Time(void);
uint32_t 	OS_TimeDifference(uint32_t Start, uint32_t Finish);
void 		OS_ClearMsTime(void);
uint32_t 	OS_MsTime(void);

// Error functions
typedef enum KernelPanicErrors
{
	HEAP_OUTOFMEMORY,
	ACTIVELINKEDLIST_ILLEGALCONNECTIONS,
	SLEEPLINKEDLIST_ILLEGALCONNECTIONS,
	LINKEDLIST_ILLEGALCONNECTIONS,
	SCHEDULER_OUTOFTHREADS,
	SCHEDULER_ILLEGALSTATE,
	SEMAPHORE_ILLEGALSTATE,
	FILESYSTEM_ERROR
} VRTOS_ErrorType;


typedef struct nRTOS_OSInfo
{
	TCB* 		pTCB_RunPt;				// The current running thread.

	uint32_t 	TimeSlice;				// The time slice for each run. This determines the SysTick interrupt.
	uint8_t 	 	PriorityLevel;			// Current foreground priority level

	bool			OS_Running;				// Indicates if the OS has launched or not

	uint64_t    msTimeCounter;			// Retains runtime counter from last OS_ClearMsTime call


} OSInfo;

struct SystemHardwareInformation
{
	unsigned long BusFrequency;
};

void OS_KernelPanic(VRTOS_ErrorType errorCode, char* Location);

// User-Level Tasks
void IdleTask(void);		// low power mode
void OS_AutoRun(void);	// Autorun task

// Intra-kernel function calls
OSInfo* 		_OS_GetOperatingSystemInformation(void);
void 		_OS_ToggleOffsetMode(void);					// Toggle the offset mode so that the context switcher uses the second link for
													// LinkedLists.
uint8_t 		_OS_GetOffsetMode(void);
void 		_OS_ToggleThreadType();						// Lets the kernel know that the next thread that is about to be added is a OS thread and doesn't require a process
ELFEnv_t* 	_OS_GetProcessEnvironment();
bool 		_OS_IsOSThread();						// Returns true if the calling thread is a OS thread; else returns false
int 			_OS_AddInitialProcessThread(void (*threadTask)(void), uint32_t stackSize, uint32_t priority, PCB* pPCB_NewProcess);

#endif /* SILK_OS_CRITICAL_OS_H_ */
