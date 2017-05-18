/*
 * mailbox.c
 *
 *  Created on: Feb 12, 2016
 *      Author: Venkat
 */

#include <stdint.h>
#include "../OS.h"
#include "../MutualExclusionManager/OS_Semaphore.h"
#include "../../Venkatware/FIFO.h"
#include "OS_Communication.h"

// Mailbox Methods

// Conditional Synchronization Semaphores
Semaphore* mailbox_Send;
Semaphore* mailbox_Ack;

// Data Mutex Protection Semaphore
Semaphore* mailbox_Mutex;

// Data Field for Mailbox
uint64_t mailbox_data;


void OS_MailBox_Init()
{
	mailbox_Send 	= OS_InitSemaphore(0);
	mailbox_Ack  	= OS_InitSemaphore(1);
	mailbox_Mutex 	= OS_InitSemaphore(1);
}

void OS_MailBox_Send(uint64_t data)
{
	OS_Wait(mailbox_Ack);
	OS_Wait(mailbox_Mutex);
	mailbox_data = data;
	OS_Signal(mailbox_Mutex);
	OS_Signal(mailbox_Send);
}

uint64_t OS_MailBox_Recv(void)
{
	/*
	uint64_t localdata;
	OS_Wait(mailbox_Send);
	OS_Wait(mailbox_Mutex);
	localdata = mailbox_data;
	OS_Signal(mailbox_Mutex);
	*/
	uint64_t localdata;
	OS_Wait(mailbox_Send);
	OS_Wait(mailbox_Mutex);
	localdata = mailbox_data;
	OS_Signal(mailbox_Mutex);
	OS_Signal(mailbox_Ack);
	return localdata;
}

// ISR FIFO (No Semaphore Synchronization)
AddIndexFifo(ISR, ISRFIFOSIZE, uint32_t, ISRFIFO_SUCCESS, ISRFIFO_FAIL)

void OS_Fifo_Init(uint32_t Size)
{
	ISRFifo_Init();
}

uint32_t OS_Fifo_Get()
{
	uint32_t result = 0;
	if (ISRFifo_Get(&result) == ISRFIFO_SUCCESS) return result;
	else return 0;
}

FIFOResult OS_Fifo_Put(uint32_t data)
{
	return (FIFOResult) ISRFifo_Put(data);
}


