/*
 * LED.c
 *
 *  Created on: Mar 17, 2016
 *      Author: Venkat
 */

#include <stdint.h>
#include "../../../OS_Critical/OS.h"
#include "../../../OS_Critical/HardwareManager/LCDDriver/ST7735.h"
#include "../../../OS_Critical/HardwareManager/LCDDriver/ST7735x.h"
#include "../../../OS_Critical/HardwareManager/SystemOscillatorDriver/PLL.h"
#include "../../../OS_Critical/HardwareManager/UARTDriver/UART.h"
#include "../../../OS_Critical/MutualExclusionManager/OS_Semaphore.h"

// External Private Hardware Functions
extern void _OS_InitializePortF(void);


#define LED_DATA_R (*((volatile unsigned long *)0x40025038))

void LED_Init()
{
	_OS_InitializePortF();
}

void LED_Out(uint8_t COLOR)
{
	  LED_DATA_R = COLOR;
}
