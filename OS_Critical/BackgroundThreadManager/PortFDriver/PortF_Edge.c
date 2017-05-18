/*
 * PortF_Edge.c
 *
 *  Created on: Feb 12, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../../../Venkatware/venkatlib.h"
#include "../../../inc/tm4c123gh6pm.h"


void (*SW1Task)(void) = 0;   // Function to run when edge triggers
void (*SW2Task)(void) = 0;   // Function to run when edge triggers


bool SwitchInitialized = FALSE;

volatile uint32_t FallingEdges = 0;
void PortFEdge_Init(uint32_t priority)
{
	  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
	  FallingEdges = 0;             // (b) initialize counter

	  // GPIO Init
	  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
	  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	  // only PF0 needs to be unlocked, other bits can't be locked
	  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
	  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
	  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
	  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
	  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
	  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0

	  // Edge trigger init
	  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4 is edge-sensitive
	  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4 is not both edges
	  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4 falling edge event
	  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flag4
	  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
	  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|((priority&0x07) << 21); // Variable priority
	  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
	  SwitchInitialized = TRUE;
}

void PortFEdge_SetFunction1(void (*switchTask)(void))
{
	SW1Task = switchTask;
}

void PortFEdge_SetFunction2(void (*switchTask)(void))
{
	SW2Task = switchTask;
}

bool PortFEdge_GetInitState()
{
	return SwitchInitialized;
}

void GPIOPortF_Handler(void)
{
	if ((GPIO_PORTF_RIS_R & 0x10) == 0x10)
	{
		if (SW1Task != 0)
		{
			(*SW1Task)();                // execute user task
		}

		GPIO_PORTF_ICR_R = 0x10;
	}
	else if ((GPIO_PORTF_RIS_R & 0x01) == 0x01)
	{
		if (SW2Task != 0)
		{
			(*SW2Task)();
		}

		GPIO_PORTF_ICR_R = 0x01;
	}
}

