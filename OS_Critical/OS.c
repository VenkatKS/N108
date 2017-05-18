/*
 * OS.c
 *
 *  Created on: Feb 29, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "MemoryManager/OS_Allocation.h"
#include "OS.h"
#include "PriorityManager/OS_Priority.h"
#include "TimedTaskManager/OS_SleepManager.h"
#include "HardwareManager/SystemOscillatorDriver/PLL.h"
#include "HardwareManager/OS_Hardware.h"
#include "../inc/tm4c123gh6pm.h"
#include "TimedTaskManager/Timer1Driver/SystemTimer.h"
#include "../Venkatware/venkatlib.h"
#include "../OS_FileSys/OS_FileSystemScheme.h"
#include "ProcessLoaderManager/loader.h"

uint32_t NumberOfThreadsCreated = 0;  // Represents the total number of threads created since OS started
uint32_t NumberOfProcessesCreated = 0;
uint32_t NumberOfActiveThreads = 0;	 // Represents the number of threads currently being churned
uint32_t NumberOfActiveProcesses = 0;

OSInfo   RunningOSInformation;

const struct SystemHardwareInformation SystemInformation = {TIME_1MS};

uint32_t OffsetMode;

extern uint32_t SVCNumberExtract();

// ASM function definitions
extern void aOS_Launch(void);
extern unsigned int GetStackPointer(void);
extern void SetStackPointer(unsigned int newSP);


// OS Device Driver Calls
static const ELFSymbol_t SymbolTable[] =
{
      {"ST7735_Message", OS_DisplayMessage}
};
ELFEnv_t nRTOS_Environment = {SymbolTable, 1};



// OS Control Functions

void OS_Init()
{
	DisableInterrupts();			// End critical in OS_Start
	NumberOfThreadsCreated 			= 0;
	NumberOfActiveThreads 			= 0;
	OffsetMode 						= 0;
	RunningOSInformation.OS_Running 	= FALSE;

	// Initialize the memory manager
	OS_AllocationInit();

	// Initialize the scheduler
	OS_PriorityInit();

	// Initialize the FileSystem
	OSFS_Init();

	// Initialize the hardware items
	OS_HardwareInit();




	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
	NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0xE0000000;

}

void OS_Launch(uint32_t timePeriod)
{
	RunningOSInformation.TimeSlice 	= timePeriod;

	RunningOSInformation.pTCB_RunPt 	= OS_GetNextThreadToRun();
	RunningOSInformation.PriorityLevel = RunningOSInformation.pTCB_RunPt->pri;
	RunningOSInformation.OS_Running    = TRUE;
	RunningOSInformation.msTimeCounter = 0;

	OS_SleepManagerInit();

	if (RunningOSInformation.pTCB_RunPt == 0)
	{
		// Panic the kernel since we don't have any threads
		OS_KernelPanic(SCHEDULER_OUTOFTHREADS, "OS_Start");
	}

	NVIC_ST_RELOAD_R = timePeriod-1;// reload value
	NVIC_ST_CTRL_R = 0x07;

	EnableInterrupts();

	aOS_Launch();
}






// Process Management
int OS_AddProcess(void(*entry)(void), void *text, void *data, unsigned long stackSize, unsigned long priority)
{
	if (NumberOfActiveThreads == 5) { return 0;}		// Max number of foreground threads

	if (text == 0 || data == 0)
	{
		//OS_SerialOutString("Error Loading Process: Not enough memory.");
		return 0;
	}

	uint32_t Status = StartCritical();

	// The new process that is being added
	PCB* pPCB_CurrentProcessBeingAdded = 0;

	pPCB_CurrentProcessBeingAdded = (PCB*) OS_NewAllocation(sizeof(PCB) * 1);

	if (pPCB_CurrentProcessBeingAdded == 0)
	{
		// Panic the kernel, let's assume for now that this should not happen
		//OS_SerialOutString("\nError Loading Process: Not enough memory.\n");
		OS_DestroyAllocation(pPCB_CurrentProcessBeingAdded, sizeof(PCB));
		OS_DestroyAllocationNonTracked(text);
		OS_DestroyAllocationNonTracked(data);
		EndCritical(Status);
		return 0;
	}

	// Unique process ID
	pPCB_CurrentProcessBeingAdded->pid = NumberOfProcessesCreated++;
	NumberOfActiveProcesses++;

	pPCB_CurrentProcessBeingAdded->CodeSegment 		= text;	 // Save the code segment area
	pPCB_CurrentProcessBeingAdded->DataSegment 		= data; 	 // Save the data segment area
	pPCB_CurrentProcessBeingAdded->ProgramCounter 	= entry; // Save the initial PC
	pPCB_CurrentProcessBeingAdded->numThreads		= 0;
	if (_OS_AddInitialProcessThread(entry, stackSize, priority, pPCB_CurrentProcessBeingAdded) == 0)
	{
		//OS_SerialOutString("\nError Loading Process: Not enough memory.\n");
		OS_DestroyAllocation(pPCB_CurrentProcessBeingAdded, sizeof(PCB));
		OS_DestroyAllocationNonTracked(text);
		OS_DestroyAllocationNonTracked(data);
		EndCritical(Status);
		return 0;
	}


	EndCritical(Status);
	return 1; //successful



}

int OS_AddThread(void (*threadTask)(void), uint32_t stackSize, uint32_t priority)
{
	if (NumberOfActiveThreads == 5)
	{
		return 0;
	}		// Max number of foreground threads


	// TODO: Replace StartCritical/EndCritical with Semaphores
	uint32_t Status = StartCritical();

	// Represents the newest thread being added
	TCB* pTCB_CurrentThreadBeingAdded = 0;

	// Allocate space for the new thread structure and its stack
	pTCB_CurrentThreadBeingAdded = (TCB*) OS_NewAllocation(sizeof(TCB) * 1);

	// if memory allocation failed, return immediately
	if (pTCB_CurrentThreadBeingAdded == 0)
	{
		EndCritical(Status);
		return 0;
	}

	// The unique ID for the thread currently being created
	// TODO: Make this atomic!
	pTCB_CurrentThreadBeingAdded->tid = NumberOfThreadsCreated;

	NumberOfThreadsCreated++;
	NumberOfActiveThreads++;

	OS_SetUpInitialStack(pTCB_CurrentThreadBeingAdded);

	pTCB_CurrentThreadBeingAdded->Thread_Stack[STACK_WORD_SIZE - 2] 	= (int) (threadTask); 			  // load the PC value

	if(RunningOSInformation.OS_Running && (!_OS_IsOSThread()))	// Not an OS thread, need to set up parent process accesses
	{
		pTCB_CurrentThreadBeingAdded->Thread_Stack[STACK_WORD_SIZE - 11]	= (int) RunningOSInformation.pTCB_RunPt->pPCB_ParentProcess->DataSegment; // Initialize the segment register (R9)
		pTCB_CurrentThreadBeingAdded->pPCB_ParentProcess = RunningOSInformation.pTCB_RunPt->pPCB_ParentProcess;
		pTCB_CurrentThreadBeingAdded->pPCB_ParentProcess->numThreads++; // increment the number of threads in this process
	}

	pTCB_CurrentThreadBeingAdded->pri = priority;

	//OS_LinkedList_InsertNodeAtEnd(RunningList, pTCB_CurrentThreadBeingAdded);
	OS_AddThreadToScheduler(pTCB_CurrentThreadBeingAdded);

	if (RunningOSInformation.OS_Running == TRUE)
	{
		NVIC_ST_CURRENT_R = 0;
		NVIC_INT_CTRL_R = 0x04000000; // Initiate manual interrupt
	}

	EndCritical(Status);
	return 1; //successful
}


void OS_SetUpInitialStack(TCB* currentThread)
{
	currentThread->pInt_CurrentSP = &currentThread->Thread_Stack[STACK_WORD_SIZE - 16];

	currentThread->Thread_Stack[STACK_WORD_SIZE - 1] 		= 0x01000000;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 3] 		= 0x14141414;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 4] 		= 0x12121212;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 5] 		= 0x03030303;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 6] 		= 0x02020202;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 7]	 	= 0x01010101;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 8] 		= 0x00000000;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 9] 		= 0x11111111;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 10] 	= 0x10101010;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 11] 	= 0x09090909;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 12] 	= 0x08080808;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 13] 	= 0x07070707;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 14] 	= 0x06060606;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 15] 	= 0x05050505;
	currentThread->Thread_Stack[STACK_WORD_SIZE - 16] 	= 0x04040404;
}




// Thread Control functions

void OS_Sleep(uint32_t SleepTimeInMillis)
{
	// TODO: Replace StartCritical and EndCritical with Semaphores
	uint32_t Status = StartCritical();
	TCB* 	currentThread 		= RunningOSInformation.pTCB_RunPt;

	//uint32_t 	TimeUnitsMultiplier 	= (SystemInformation.BusFrequency * 1000) / RunningOSInformation.TimeSlice;

	currentThread->SleepTimeUnitsLeft = SleepTimeInMillis;

	//if (Heap_Test() == 1) OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_Sleep");

	OS_RemoveThreadFromScheduler(currentThread); // I am corrupting the heap somewhere in here

	//if (Heap_Test() == 1) OS_KernelPanic(HEAP_OUTOFMEMORY, "OS_Sleep");

	OS_NewSleepingThread(currentThread);

	NVIC_ST_CURRENT_R = 0;
	NVIC_INT_CTRL_R = 0x04000000; // Initiate manual interrupt
	EndCritical(Status);
}

void OS_Kill(void)
{
	// TODO: Replace StartCritical and EndCritical with Semaphores
	uint32_t Status = StartCritical();
	TCB* 	currentThread 		= RunningOSInformation.pTCB_RunPt;

	OS_RemoveThreadFromScheduler(currentThread); // I am corrupting the heap somewhere in here

	currentThread->pPCB_ParentProcess->numThreads--;
	if ((!_OS_IsOSThread()) && currentThread->pPCB_ParentProcess->numThreads == 0)
	{
		// Kill the process by freeing memory
		OS_DestroyAllocationNonTracked(currentThread->pPCB_ParentProcess->CodeSegment);
		OS_DestroyAllocationNonTracked(currentThread->pPCB_ParentProcess->DataSegment);
		OS_DestroyAllocation(currentThread->pPCB_ParentProcess, sizeof(PCB));

		NumberOfActiveProcesses--;
	}

	OS_DestroyAllocation(currentThread, sizeof(TCB));
	NumberOfActiveThreads--;
	NVIC_ST_CURRENT_R = 0;
	NVIC_INT_CTRL_R = 0x04000000; // Initiate manual interrupt
	EndCritical(Status);
}

void OS_Suspend()
{
	NVIC_ST_CURRENT_R = 0;
	NVIC_INT_CTRL_R = 0x04000000; // Initiate manual interrupt
}

// Thread Information Functions

uint32_t OS_Id(void)
{
	return RunningOSInformation.pTCB_RunPt->tid;
}

uint32_t OS_Time(void)
{
	return SystemTimer_GetCurrentTime();
}

uint32_t OS_TimeDifference(uint32_t Start, uint32_t Finish)
{
	return Finish - Start;
}

void OS_ClearMsTime(void)
{
	RunningOSInformation.msTimeCounter = OS_Time();
}

uint32_t OS_MsTime(void)
{
	uint64_t CurrentTime 	= OS_Time();
	uint64_t Diff			= CurrentTime - RunningOSInformation.msTimeCounter;

	return Diff / (UNITSOF20NSIN1MS);
}

// Low Power user-level IDLE task
void IdleTask(void)
{
     for(;;)
     {
    	 	 //PF2 ^= 0x02;
    	 	// __asm("\n\tWFI");
     };
}

// Button task
char AutoRunTask[] = "autorun.axf";
void OS_AutoRun(void)
{
	OS_SerialOutString("\nRunning Button Task...\n");
	if (!exec_elf(AutoRunTask, _OS_GetProcessEnvironment()))
	{
		OS_SerialOutString("Error: Out of Memory or File System Error");
	}

}

// Intra-Kernel Functions

// Get the OS Information structure
OSInfo* _OS_GetOperatingSystemInformation(void)
{
	return &RunningOSInformation;
}

// Toggle the type of offset used
void _OS_ToggleOffsetMode(void)
{
	OffsetMode = (OffsetMode == REGULAR) ? BACKUP : REGULAR;
}

// Get the current offset mode used by the OS
uint8_t _OS_GetOffsetMode(void)
{
	return OffsetMode;
}


ELFEnv_t* _OS_GetProcessEnvironment()
{
	return &nRTOS_Environment;
}

bool _OS_IsOSThread()
{
	return (RunningOSInformation.pTCB_RunPt->pPCB_ParentProcess == 0) ? TRUE : FALSE;
}

// Adding the first new thread for a brand new process
int _OS_AddInitialProcessThread(void (*threadTask)(void), uint32_t stackSize, uint32_t priority, PCB* pPCB_NewProcess)
{
	if (NumberOfActiveThreads == 5) { return 0;}		// Max number of foreground threads


	// TODO: Replace StartCritical/EndCritical with Semaphores
	uint32_t Status = StartCritical();

	// Represents the newest thread being added
	TCB* pTCB_CurrentThreadBeingAdded = 0;

	// Allocate space for the new thread structure and its stack
	pTCB_CurrentThreadBeingAdded = (TCB*) OS_NewAllocation(sizeof(TCB) * 1);

	// if memory allocation failed, return immediately
	if (pTCB_CurrentThreadBeingAdded == 0)
	{
		return 0;
	}

	// The unique ID for the thread currently being created
	// TODO: Make this atomic!
	pTCB_CurrentThreadBeingAdded->tid = NumberOfThreadsCreated;

	NumberOfThreadsCreated++;
	NumberOfActiveThreads++;

	OS_SetUpInitialStack(pTCB_CurrentThreadBeingAdded);

	pTCB_CurrentThreadBeingAdded->Thread_Stack[STACK_WORD_SIZE - 2] 	= (int) (threadTask); 			  // load the PC value

	pTCB_CurrentThreadBeingAdded->Thread_Stack[STACK_WORD_SIZE - 11]	= (int) pPCB_NewProcess->DataSegment; // Initialize the segment register (R9)
	pTCB_CurrentThreadBeingAdded->pPCB_ParentProcess = pPCB_NewProcess;
	pPCB_NewProcess->numThreads++; // increment the number of threads in this process

	pTCB_CurrentThreadBeingAdded->pri = priority;

	//OS_LinkedList_InsertNodeAtEnd(RunningList, pTCB_CurrentThreadBeingAdded);
	OS_AddThreadToScheduler(pTCB_CurrentThreadBeingAdded);

	if (RunningOSInformation.OS_Running == TRUE)
	{
		NVIC_ST_CURRENT_R = 0;
		NVIC_INT_CTRL_R = 0x04000000; // Initiate manual interrupt
	}

	EndCritical(Status);
	return 1; //successful
}


// Kernel Panic function
// Come here whenever something goes really wrong
void OS_KernelPanic(VRTOS_ErrorType errorCode, char* Location)
{
	// Error location is on the stack
	uint64_t KernelPanicError = errorCode;
	while(TRUE)
	{
		KernelPanicError = KernelPanicError;
	}
}

void ToggleIt()
{
	 GPIO_PORTF_DATA_R ^= 0x02;
}
