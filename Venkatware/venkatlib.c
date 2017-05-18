/*
 * venkatlib.c
 *
 *  Created on: Feb 22, 2016
 *      Author: Venkat
 */

#include <stdlib.h>
#include <stdint.h>
#include "venkatlib.h"
char* integerToString(int i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}


#define NANOINSECONDS 1000000000
#define FIXEDPOINT 1000000
int TimerFrequencyToPeriod50MHZPLL(int Freq)
{
	return (NANOINSECONDS/Freq)/mhz50timer;
}
