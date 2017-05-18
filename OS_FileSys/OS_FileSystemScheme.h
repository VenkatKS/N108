/*
 * OS_FileSystemScheme.h
 *
 *  Created on: Apr 1, 2016
 *      Author: Venkat
 */

#ifndef SILK_OS_FILESYS_OS_FILESYSTEMSCHEME_H_
#define SILK_OS_FILESYS_OS_FILESYSTEMSCHEME_H_

#ifndef _FATFS
#include "ff.h"
#endif


void 		OSFS_Init();

/*
int32_t    	OSFS_Read(FILE* fileDescriptor, BYTE* Buffer, uint32_t numBytes, uint32_t Offset);
bool 		OSFS_Append(FILE* fileDescriptor, BYTE* buffer, uint32_t numBytes);
bool 		OSFS_Delete(char* fileName);
*/

#endif /* SILK_OS_FILESYS_OS_FILESYSTEMSCHEME_H_ */
