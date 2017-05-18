/*
 * mailbox.h
 *
 *  Created on: Feb 12, 2016
 *      Author: Venkat
 */

#ifndef OS_CRITICAL_THREAD_COMMUNICATION_SYNCHRONOUSMAILBOX_H_
#define OS_CRITICAL_THREAD_COMMUNICATION_SYNCHRONOUSMAILBOX_H_

void 		OS_MailBox_Init(void);
void 		OS_MailBox_Send(uint64_t data);
uint64_t 	OS_MailBox_Recv(void);
uint8_t 		OS_Fifo_PutNonBlocking(int32_t Data);


// ISR FIFO Functions (No Semaphore Synchronization) -- needs lockfree synch
typedef enum ISRFifoConditions
{
	ISRFIFO_FAIL,
	ISRFIFO_SUCCESS
} FIFOResult;
#define ISRFIFOSIZE 32

void 		OS_Fifo_Init(uint32_t Size);
uint32_t 	OS_Fifo_Get(void);
FIFOResult 	OS_Fifo_Put(uint32_t data);

#endif /* OS_CRITICAL_THREAD_COMMUNICATION_SYNCHRONOUSMAILBOX_H_ */
