/*
 * LED.h
 *
 *  Created on: Mar 17, 2016
 *      Author: Venkat
 */

#ifndef OS_KERNEL_HARDWAREMANAGER_LEDDRIVER_LED_H_
#define OS_KERNEL_HARDWAREMANAGER_LEDDRIVER_LED_H_

#define LED_RED       	0x02
#define LED_BLUE      	0x04
#define LED_GREEN     	0x08
#define LED_OFF			0x00

void LED_Init(void);
void LED_Out(uint8_t COLOR);



#endif /* OS_KERNEL_HARDWAREMANAGER_LEDDRIVER_LED_H_ */
