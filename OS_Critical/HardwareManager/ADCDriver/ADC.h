/*
 * ADC.h
 *
 *  Created on: Feb 23, 2016
 *      Author: Venkat
 */


uint8_t 		_ADC_Open(uint8_t ChannelNumber);
void 		_ADC_Collect(uint32_t ChannelNum, uint32_t Period, void(*ADCTask)(uint32_t data));
uint32_t 	_ADC_In();
