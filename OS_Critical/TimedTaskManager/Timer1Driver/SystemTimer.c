// Timer1.c
// Runs on LM4F120/TM4C123
// Use TIMER1 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// May 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include <stdint.h>
#include "../../../inc/tm4c123gh6pm.h"
#include "../OS_SleepManager.h"

#define NSPERCYCLEFOR50MHZ 20

uint64_t SystemClock = 0;
uint32_t TimerPeriod = 0;
void SystemTimer_Init(uint32_t period)  // the time slice given by the operating system
{
	  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
	  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
	  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	  TIMER1_TAILR_R = period-1;    // 4) reload value
	  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
	  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
	  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00000000; // 8) priority 0, needs to be top priority
	  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
	  TimerPeriod = period - 1;
}

void Timer1A_Handler(void)
{
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	_INTERNAL_OS_SleepManager();
	SystemClock++;
}


uint64_t SystemTimer_GetCurrentTime()
{
	uint64_t 	NormalizedTime = ((SystemClock*TimerPeriod) + ((TimerPeriod - TIMER1_TAV_R)));

	return 		NormalizedTime;
}


