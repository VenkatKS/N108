/*
 * LinkedList.h
 *
 *  Created on: Feb 19, 2016
 *      Author: Venkat
 */

#ifndef LAB3_VRTOS_OS_CRITICAL_OS_LINKEDLIST_OS_LINKEDLIST_H_
#define LAB3_VRTOS_OS_CRITICAL_OS_LINKEDLIST_OS_LINKEDLIST_H_

typedef struct nRTOS_LinkedList
{
	TCB* pTCB_Head;
	TCB* pTCB_Tail;
	uint32_t NumberOfNodes;
	uint32_t ActiveNode;
	// TODO: Add semaphore here
} ThreadQueue;


ThreadQueue* OS_LinkedList_NewLinkedList();
void OS_LinkedList_InsertNodeAtEnd(ThreadQueue* pQ_Host, TCB* pTCB_NodeToInsert);
void OS_LinkedList_RemoveNode(ThreadQueue* pQ_Host, TCB* pTCB_NodeToDelete);





#endif /* LAB3_VRTOS_OS_CRITICAL_OS_LINKEDLIST_OS_LINKEDLIST_H_ */
