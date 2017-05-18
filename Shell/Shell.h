/*
 * Shell.h
 *
 *  Created on: Feb 1, 2016
 *      Author: Venkat
 */

#ifndef OPERATING_SYSTEM_SHELL_H_
#define OPERATING_SYSTEM_SHELL_H_

#define COMMAND_MAX_SIZE 200
#define PARAMS_MAX_NUM   10
#define PARAMS_MAX_SIZE  20

// Serial Tokens
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F




struct CommandLine
{
	char 				UnFormatted[COMMAND_MAX_SIZE];
	char 				Formatted[COMMAND_MAX_SIZE];

	char*				Tokens;
};

void 	Interpreter(void);
int 		Shell_CommandNumber(char* commandString);
void 	Shell_StringRegex(char* src, char* dst); // Null terminated strings
void 	Shell_FreeTokens(char** tokens);
void 	Shell_CommandTokenize(char* pCommand);
void 	Shell_RunCommand(unsigned int Index);



#endif /* OPERATING_SYSTEM_SHELL_H_ */
