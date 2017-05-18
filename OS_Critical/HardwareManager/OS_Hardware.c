/*
 * OS_Hardware.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../OS.h"
#include "SystemOscillatorDriver/PLL.h"
#include "LEDDriver/LED.h"
#include "UARTDriver/UART.h"
#include "ADCDriver/ADC.h"
#include "LCDDriver/ST7735.h"
#include "LCDDriver/ST7735x.h"
#include "../MutualExclusionManager/OS_Semaphore.h"

Semaphore* LCD_Sem4;

void OS_HardwareInit(void)
{
	PLL_Init(Bus50MHz);
	UART_Init();
	Output_Init();
	LED_Init();
	LCD_Sem4 = OS_InitSemaphore(1);
}



// Shell functions
void OS_SerialOutString(char* string)
{
	UART_OutString(string);
}

void OS_SerialOutChar(char data)
{
	UART_OutChar(data);
}

void OS_SerialOutUDec(uint32_t dec)
{
	UART_OutUDec(dec);
}

void OS_SerialIn(char* bufferArray, uint16_t bufferSize)
{
	UART_InString(bufferArray, bufferSize);
}

void OS_SerialOutMessage(unsigned int device, unsigned int line, char *string, unsigned long value)
{
	OS_Wait(LCD_Sem4);
	OS_SerialOutString("\n::::::USER TASK: ");
	OS_SerialOutString(string);
	OS_SerialOutString(" ");
	OS_SerialOutUDec(value);
	OS_SerialOutString(" ");
	OS_SerialOutString("::::::\n");
	OS_Signal(LCD_Sem4);

}

// LCD Functions
void OS_DisplayOutString(char* string)
{
	OS_Wait(LCD_Sem4);
	ST7735_OutString(string);
	OS_Signal(LCD_Sem4);
}

void OS_DisplayOutChar(char c)
{
	OS_Wait(LCD_Sem4);
	ST7735_OutChar(c);
	OS_Signal(LCD_Sem4);
}

void OS_DisplayOutUDec(uint32_t dec)
{
	OS_Wait(LCD_Sem4);
	ST7735_OutUDec(dec);
	OS_Signal(LCD_Sem4);
}

void OS_DisplayMessage(unsigned int device, unsigned int line, char *string, unsigned long value)
{
	OS_Wait(LCD_Sem4);
	ST7735_Message(device, line, string, value);
	OS_Signal(LCD_Sem4);
}

// ADC Functions
uint8_t 	ADC_Open(uint8_t ChannelNumber)
{
	return _ADC_Open(ChannelNumber);
}

void ADC_Collect(uint32_t ChannelNum, uint32_t Period, void(*ADCTask)(uint32_t data))
{
	_ADC_Collect(ChannelNum, Period, ADCTask);
}

uint32_t ADC_In()
{
	return _ADC_In();
}



