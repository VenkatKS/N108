/*
 * main.c
 */

#include <stdint.h>
#include "OS_Critical/OS.h"
#include "OS_Critical/MutualExclusionManager/OS_Semaphore.h"
#include "OS_Critical/MemoryManager/heap.h"
#include "Shell/Shell.h"
#include "OS_Critical/BackgroundThreadManager/OS_BackgroundThread.h"
#include "Venkatware/FIFO.h"
#include "OS_Critical/HardwareManager/OS_Hardware.h"
#include "OS_Critical/CommunicationManager/OS_Communication.h"
#include "OS_Critical/EvaluationManager/Jitter.h"
#include "inc/tm4c123gh6pm.h"
#include "ProjectControl.h"

uint32_t Count1;


#ifdef LAB3_MAIN
#define PE0  (*((volatile unsigned long *)0x40024004))
#define PE1  (*((volatile unsigned long *)0x40024008))
#define PE2  (*((volatile unsigned long *)0x40024010))
#define PE3  (*((volatile unsigned long *)0x40024020))


Semaphore* ThreadSem;

uint32_t Done = 0;
void Thread1(void)
{
	Count1 = 0;
	for(;;)
	{
		//OS_DisplayOutString("Thread 1 running\n");
		Count1++;
	}
}

uint32_t Count2;
void Thread2()
{
	Count2 = 0;
	for(;;)
	{
		OS_DisplayOutString("Thread 2 running\n");
		Count2++;
		OS_Sleep(1000);
	}
}

uint32_t Count3;
void Thread3()
{
	Count3 = 0;
	for(;;)
	{
		OS_DisplayOutString("Thread 3 running\n");
		Count3++;
		OS_Sleep(1000);
	}
}

uint32_t Count4 = 0;
void Thread4()
{
	Count4++;
}

uint32_t Count5 = 0;
void Thread5()
{
	Count5++;
}


int myMain(void)
{
	OS_Init();
	OS_AddThread(&Thread2, 100, 3);
	OS_AddThread(&Thread3, 100, 3);
	OS_AddThread(&Thread1, 100, 4);
	OS_AddThread(&Interpreter, 100, 4);

	OS_AddPeriodicThread(Thread4, TIME_1MS, 4);
	OS_AddPeriodicThread(Thread5, TIME_1MS, 5);

	ThreadSem = OS_InitSemaphore(1);


	OS_Launch(TIME_1MS/10);
	
	return 0;
}


// TestMain5

//******************* Lab 3 Preparation 2**********
// Modify this so it runs with your RTOS (i.e., fix the time units to match your OS)
// run this with
// UART0, 115200 baud rate, used to output results
// SYSTICK interrupts, period established by OS_Launch
// first timer interrupts, period established by first call to OS_AddPeriodicThread
// second timer interrupts, period established by second call to OS_AddPeriodicThread
// SW1 no interrupts
// SW2 no interrupts
unsigned long CountA;   // number of times Task A called
unsigned long CountB;   // number of times Task B called


//*******PseudoWork*************
// simple time delay, simulates user program doing real work
// Input: amount of work in 100ns units (free free to change units
// Output: none
uint32_t NumCreated;
void PseudoWork(unsigned short work){
unsigned short startTime;
  startTime = OS_Time();    // time in 20ns units
  while(OS_TimeDifference(startTime,OS_Time()) <= work*5){}
}
void Thread6(void){  // foreground thread
  Count1 = 0;
  for(;;){
    Count1++;
  }
}

extern void Jitter(void);   // prints jitter information (write this)
void Thread7(void){  // foreground thread
  OS_SerialOutString("\n\rEE345M/EE380L, Lab 3 Preparation 2\n\r");
  OS_Sleep(5000);   // 10 seconds
  JittertPrint();
  OS_SerialOutString("\n\r\n\r");
  OS_Kill();
}
#define workA 500       // {5,50,500 us} work in Task A
#define counts1us 10    // number of OS_Time counts per 1us
uint32_t counttime = 0;
uint64_t time1 = 0;
void TaskA(void){       // called every {1000, 2990us} in background
  CountA++;
  Jitter();
  PseudoWork(workA*counts1us); //  do work (100ns time resolution)
}
#define workB 250       // 250 us work in Task B
void TaskB(void){       // called every pB in background
  CountB++;
  PseudoWork(workB*counts1us); //  do work (100ns time resolution)
}

int mainItLast(void){       // Testmain5 Lab 3
  OS_Init();           // initialize, disable interrupts
  NumCreated = 0 ;
  NumCreated += OS_AddThread(&Thread6,128,2);
  NumCreated += OS_AddThread(&Thread7,128,1);
  OS_AddPeriodicThread(&TaskA,TIME_500US,0);           // 1 ms, higher priority
  OS_AddPeriodicThread(&TaskB,2*TIME_1MS,1);         // 2 ms, lower priority

  OS_Launch(TIME_2MS); // 2ms, doesn't return, interrupts enabled in here
  return 0;             // this never executes
}

////////////////



//Test Main 6//

//******************* Lab 3 Preparation 4**********
// Modify this so it runs with your RTOS used to test blocking semaphores
// run this with
// UART0, 115200 baud rate,  used to output results
// SYSTICK interrupts, period established by OS_Launch
// first timer interrupts, period established by first call to OS_AddPeriodicThread
// second timer interrupts, period established by second call to OS_AddPeriodicThread
// SW1 no interrupts,
// SW2 no interrupts
void PortE_Init(void){ unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x10;       // activate port E
  delay = SYSCTL_RCGC2_R;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTE_DIR_R |= 0x0F;    // make PE3-0 output heartbeats
  GPIO_PORTE_AFSEL_R &= ~0x0F;   // disable alt funct on PE3-0
  GPIO_PORTE_DEN_R |= 0x0F;     // enable digital I/O on PE3-0
  GPIO_PORTE_PCTL_R = ~0x0000FFFF;
  GPIO_PORTE_AMSEL_R &= ~0x0F;;      // disable analog functionality on PF
}

Semaphore* s;            // test of this counting semaphore
unsigned long SignalCount1;   // number of times s is signaled
unsigned long SignalCount2;   // number of times s is signaled
unsigned long SignalCount3;   // number of times s is signaled
unsigned long WaitCount1;     // number of times s is successfully waited on
unsigned long WaitCount2;     // number of times s is successfully waited on
unsigned long WaitCount3;     // number of times s is successfully waited on
#define MAXCOUNT 20000
void OutputThread(void){  // foreground thread
  OS_SerialOutString("\n\rEE345M/EE380L, Lab 3 Preparation 4\n\r");
  while(SignalCount1+SignalCount2+SignalCount3<100*MAXCOUNT){
    OS_Sleep(1000);   // 1 second
    OS_SerialOutString(".");
  }
  OS_SerialOutString(" done\n\r");
  OS_SerialOutString("Signalled="); OS_SerialOutUDec(SignalCount1+SignalCount2+SignalCount3);
  OS_SerialOutString(", Waited="); OS_SerialOutUDec(WaitCount1+WaitCount2+WaitCount3);
  OS_SerialOutString("\n\r");
  OS_Kill();
}
void Wait1(void){  // foreground thread
  for(;;){
    OS_Wait(s);    // three threads waiting
    PE1 ^=0x02;
    WaitCount1++;
  }
}
void Wait2(void){  // foreground thread
  for(;;){
    OS_Wait(s);    // three threads waiting
    PE2 ^=0x04;
    WaitCount2++;
  }
}
void Wait3(void){   // foreground thread
  for(;;){
    OS_Wait(s);    // three threads waiting
    PE3 ^= 0x08;
    WaitCount3++;
  }
}
void Signal1(void){      // called every 799us in background
  if(SignalCount1<MAXCOUNT){
    OS_Signal(s);
    SignalCount1++;
  }
}
// edit this so it changes the periodic rate
void Signal2(void){       // called every 1111us in background
  if(SignalCount2<MAXCOUNT){
    OS_Signal(s);
    SignalCount2++;
  }
}
void Signal3(void){       // foreground
  while(SignalCount3<98*MAXCOUNT){
    OS_Signal(s);
    SignalCount3++;
  }
  OS_Kill();
}

long add(const long n, const long m){
static long result;
  result = m+n;
  return result;
}

int main(void){      // Testmain6  Lab 3
  volatile unsigned long delay;
  OS_Init();           // initialize, disable interrupts
  PortE_Init();
  delay = add(3,4);
  SignalCount1 = 0;   // number of times s is signaled
  SignalCount2 = 0;   // number of times s is signaled
  SignalCount3 = 0;   // number of times s is signaled

  WaitCount1 = 0;     // number of times s is successfully waited on
  WaitCount2 = 0;     // number of times s is successfully waited on
  WaitCount3 = 0;	  // number of times s is successfully waited on

  s = OS_InitSemaphore(0);	 // this is the test semaphore
  OS_AddPeriodicThread(&Signal1,(799*TIME_1MS)/1000,0);   // 0.799 ms, higher priority
  OS_AddPeriodicThread(&Signal2,(1111*TIME_1MS)/1000,1);  // 1.111 ms, lower priority


  NumCreated = 0 ;
  NumCreated += OS_AddThread(&Thread6,128,6);    	// idle thread to keep from crashing
  NumCreated += OS_AddThread(&OutputThread,128,2); 	// results output thread
  NumCreated += OS_AddThread(&Signal3,128,2); 	// signalling thread
  NumCreated += OS_AddThread(&Wait1,128,2); 	// waiting thread
  NumCreated += OS_AddThread(&Wait2,128,2); 	// waiting thread
  NumCreated += OS_AddThread(&Wait3,128,2); 	// waiting thread

  OS_Launch(TIME_1MS);  // 1ms, doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
#endif
