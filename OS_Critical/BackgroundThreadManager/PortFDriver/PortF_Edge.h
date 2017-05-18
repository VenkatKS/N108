/*
 * PortF_Edge.h
 *
 *  Created on: Feb 12, 2016
 *      Author: Venkat
 */

#ifndef GPIO_DRIVERS_PORTF_DRIVER_PORTF_EDGE_H_
#define GPIO_DRIVERS_PORTF_DRIVER_PORTF_EDGE_H_
#ifndef LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_
#include "../../../Venkatware/venkatlib.h"
#endif

void PortFEdge_Init(uint32_t priority);
void PortFEdge_SetFunction1(void (*switchTask)(void));
void PortFEdge_SetFunction2(void (*switchTask)(void));
bool PortFEdge_GetInitState();

void GPIOPortF_Handler(void);


#endif /* GPIO_DRIVERS_PORTF_DRIVER_PORTF_EDGE_H_ */
