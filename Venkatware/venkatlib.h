/*
 * venkatlib.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Venkat
 */

#ifndef LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_
#define LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_

char* integerToString(int i, char b[]);
int TimerFrequencyToPeriod50MHZPLL(int Freq); // 50 mhz
#define mhz50timer  20
#define NSINMS      1000000

#define UNITSOF20NSIN1MS 50000

extern long StartCritical (void);    // previous I bit, disable interrupts
extern void EndCritical(long sr);    // restore I bit to previous value
extern void EnableInterrupts(void);
extern void DisableInterrupts(void);

typedef enum tf
{
	FALSE,
	TRUE
} bool;

#endif /* LAB3_VRTOS_EXTERNAL_LIBRARIES_VENKATWARE_VENKATLIB_H_ */
