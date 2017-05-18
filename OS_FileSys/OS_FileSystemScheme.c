/*
 * OS_FileSystemScheme.c
 *
 *  Created on: Apr 1, 2016
 *      Author: Venkat
 */
#include <stdint.h>
#include <string.h>
#include "../OS_Critical/OS.h"
#include "../OS_Critical/MemoryManager/OS_Allocation.h"
#include "ff.h"
#include "OS_FileSystemScheme.h"

static FATFS g_sFatFs;
FRESULT FileAccessResult;
FRESULT MountFresult;

void OSFS_Init()
{
	MountFresult = f_mount(&g_sFatFs, "", 0);
	if(MountFresult)
	{
		OS_KernelPanic(FILESYSTEM_ERROR, "could not mount filesys");
	}



}

