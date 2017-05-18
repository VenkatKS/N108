/*
 * Shell.c
 *
 *  Created on: Feb 1, 2016
 *      Author: Venkat
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../OS_Critical/ProcessLoaderManager/loader.h"
#include "Shell.h"
#include "../OS_Critical/HardwareManager/OS_Hardware.h"
#include "../OS_FileSys/OS_FileSystemScheme.h"
#include "../OS_Critical/OS.h"
#include "../OS_FileSys/OS_FileSystemScheme.h"

#define COMMAND_COUNT 2

typedef void (*fp)(int); //Declares a type of a void function that accepts an int
extern void OutCRLF(void);

// Command Functions
void Help_Output(int one);
void Shell_LoadProcess(int one);

char*			commandDef[]			=		{
												"help: 				Output command information.",
												"runproc:			Runs the corresponding process."
											};
char* 			commandFormat[]		= 		{
												"help",
												"runproc <processname>"
											};
char* 			commands[] 			= 		{
												"help",
												"runproc"
											};

fp 				function_array[] 	= 		{
												Help_Output,
												Shell_LoadProcess
											};

unsigned int		CommandCount[]	    =       {
												0,
												1
											};

char CommandTokens[PARAMS_MAX_NUM][PARAMS_MAX_SIZE];
unsigned int CurrentCommandParamCount;



char Command[COMMAND_MAX_SIZE];
char ExecuteName[PARAMS_MAX_SIZE];


void OutCRLF(void)
{
	OS_SerialOutChar(CR);
	OS_SerialOutChar(LF);
}

void Interpreter()
{
	  OS_SerialOutString("Please enter command:");
	  //ADC_Open(0, 1000);
	  OutCRLF();
	  while (1)
	  {
		  OS_SerialIn(Command, COMMAND_MAX_SIZE);

		  //uint32_t PreviousState = StartCritical();
		  Shell_CommandTokenize(Command);

		  Shell_StringRegex(CommandTokens[0], ExecuteName);
		  int match = Shell_CommandNumber(ExecuteName);
		  if (match >= 0)
		  {
			  Shell_RunCommand(match);
		  }
		  else if (match == -1)
		  {
			  OutCRLF();
			  OS_SerialOutString("Command not found.");
		  }
		  else
		  {
				OutCRLF();
				OS_SerialOutString("ERROR: Command parameters not correct.");
		  }

		  OutCRLF();
		  OutCRLF();
		  //EndCritical(PreviousState);

		  OS_SerialOutString("Please enter command:");
	  }

}

void Shell_CommandTokenize(char* pCommand)
{
	memset(CommandTokens, 0, sizeof(CommandTokens[0][0]) * PARAMS_MAX_NUM * PARAMS_MAX_SIZE);

	unsigned int TokenIterator = 0;

	char* Token = strtok(pCommand, " ");

	while (Token)
	{

		strcpy(CommandTokens[TokenIterator], Token);

		Token = strtok(NULL, " ");

	    TokenIterator++;
	}

	CurrentCommandParamCount = TokenIterator - 1; // Do not count the command name itself

}

void Shell_FreeTokens(char** tokens)
{
	char** token_back = tokens;

	while(*tokens)
	{
		free(*tokens);
		tokens++;
	}

	free(token_back);

}

int Shell_CommandNumber(char* commandString)
{
	int fnIterator = 0;
	for (fnIterator = 0; fnIterator < COMMAND_COUNT; fnIterator++)
	{
		unsigned int compare = strcmp(commandString, commands[fnIterator]);
		if (compare == 0)
		{
			if (CurrentCommandParamCount != CommandCount[fnIterator])
			{
				return -2;
			}
			return fnIterator;
		}
	}

	return -1;
}

void Shell_StringRegex(char* src, char* dst) // Null terminated strings
{
	for (; *src; src++) {
	   if ('a' <= *src && *src <= 'z'
	    || '0' <= *src && *src <= '9'
	    || *src == '_') *dst++ = *src;
	}
	*dst = '\0';
}

void Shell_RunCommand(unsigned int Index)
{
	function_array[Index](0);
}

void Help_Output(int one)
{
	int i = 0;
	OutCRLF();
	OutCRLF();
	OS_SerialOutString("Command Information:");
	OutCRLF();
	for (i = 0; i < COMMAND_COUNT; i++)
	{
		OS_SerialOutString(commandDef[i]);
	}

	OS_SerialOutString("Command Formats:");
	OutCRLF();
	for (i = 0; i < COMMAND_COUNT; i++)
	{
		OS_SerialOutString(commandFormat[i]);
	}
}

void Shell_LoadProcess(int one)
{
	  OS_SerialOutString("\n");
	  FIL Handle;
	  char c;
	  UINT succ;
	  FRESULT Fresult = f_open(&Handle, "lab5.txt", FA_READ);
	  if(Fresult == FR_OK)
	  {
		    Fresult = f_read(&Handle, &c, 1, &succ);
		    while((Fresult == FR_OK) && (succ == 1)){
		      if(c == '\n')
		      {
		    	  	  OS_SerialOutString("\n");
		      } else
		      {
		    	  	  OS_SerialOutChar(c);
		      }
		      // get the next character in 'c'
		      Fresult = f_read(&Handle, &c, 1, &succ);
		    }
		    // close the file
		    Fresult = f_close(&Handle);
	  }

	  OS_SerialOutString("\n");

	if (exec_elf(CommandTokens[1], _OS_GetProcessEnvironment()) < 0)
	{
		OS_SerialOutString("Error: Out of Memory or File System Error.");
	}

}
