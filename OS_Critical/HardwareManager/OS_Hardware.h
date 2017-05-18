/*
 * OS_Hardware.h
 *
 *  Created on: Mar 3, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_CRITICAL_HARDWAREMANAGER_OS_HARDWARE_H_
#define SILK_OS_CRITICAL_HARDWAREMANAGER_OS_HARDWARE_H_



// Hardware initializations
void OS_HardwareInit(void);

// UART Serial Functions
void OS_SerialOutString(char* string);
void OS_SerialOutChar(char data);
void OS_SerialOutUDec(uint32_t dec);
void OS_SerialIn(char* bufferArray, uint16_t bufferSize);
void OS_SerialOutMessage(unsigned int device, unsigned int line, char *string, unsigned long value);

// LCD Output Functions
void OS_DisplayOutString(char* string);
void OS_DisplayOutChar(char c);
void OS_DisplayOutUDec(uint32_t dec);
void OS_DisplayMessage(unsigned int device, unsigned int line, char *string, unsigned long value);

// ADC Functions
uint8_t 		ADC_Open(uint8_t ChannelNumber);
void 		ADC_Collect(uint32_t ChannelNum, uint32_t Period, void(*ADCTask)(uint32_t data));
uint32_t 	ADC_In();


#endif /* SILK_OS_CRITICAL_HARDWAREMANAGER_OS_HARDWARE_H_ */
