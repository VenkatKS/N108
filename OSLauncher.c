/*
 * OSLauncher.c
 *
 *  Created on: Apr 2, 2016
 *      Author: Venkat
 */

#include "ProjectControl.h"

#ifdef MAIN_OS

#include <stdint.h>
#include "OS_Critical/OS.h"
#include "Shell/Shell.h"
#include "OS_Critical/BackgroundThreadManager/OS_BackgroundThread.h"


int main()
{
	OS_Init();
	OS_AddThread(IdleTask, 100, 5);
	OS_AddThread(Interpreter, 100, 4);
	OS_AddSW1Task(OS_AutoRun, 3);
	OS_Launch(TIME_1MS);
}
#endif
