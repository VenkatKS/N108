/*
 * Jitter.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include "../OS.h"
#include "../HardwareManager/OS_Hardware.h"

uint64_t CurrentTime;
uint64_t OldTime;

uint32_t FirstRun = 1;

uint32_t CurrentJitter;
uint32_t RunningMaxJitter;

void Jitter()
{
	if (FirstRun)
	{
		OS_ClearMsTime();
		FirstRun = 0;
	}
	CurrentTime = OS_Time();
	if(OS_MsTime()<10000)  // finite run time
	{
	  unsigned long diff = OS_TimeDifference(OldTime,CurrentTime);
	  if(diff>TIME_500US){
		  CurrentJitter = (diff-TIME_500US+4)/8;  // in 0.1 usec
	  }else{
		  CurrentJitter = (TIME_500US-diff+4)/8;  // in 0.1 usec
	  }
	  if(CurrentJitter > RunningMaxJitter){
		RunningMaxJitter = CurrentJitter; // in usec
	  }       // jitter should be 0
	  if(CurrentJitter >= 64){
		CurrentJitter = 64-1;
	  }

	}

	OldTime = CurrentTime;
}

void JittertPrint()
{
	OS_SerialOutUDec(RunningMaxJitter);
}
