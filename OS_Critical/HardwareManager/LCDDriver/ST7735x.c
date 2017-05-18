

// ST7735 Extension library

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ST7735.h"
#include "ST7735x.h"
#include "../../OS.h"
#include "../../MutualExclusionManager/OS_Semaphore.h"
#include "../../../Venkatware/venkatlib.h"

// Belongs to ST7735 module
// System properties:
// Top Screen: lines 0 to 7
// Bottom Screen: lines 8 to 15

// device: {0: top, 1: bottom}
// line:   {0-7}

#define TOP_LCD_HEIGHT 		ST7735_TFTHEIGHT/2   // Should be 80
#define BOTTOM_LCD_HEIGHT 	ST7735_TFTHEIGHT    	// Should be 160

unsigned short us_g_CurrentColor = ST7735_WHITE;

void LCD_Init()
{
	Output_Init();

}

void ST7735x_SetCurrentDefaultColor(unsigned short us_p_newColor)
{
	us_g_CurrentColor = us_p_newColor;
}

char valueString[10];
void ST7735_Message (unsigned int device, unsigned int line, char *string, unsigned long value)
{
	integerToString(value, valueString);

	unsigned int ui_ConvertedLCDLine = 0;
	
	ui_ConvertedLCDLine = 	(device == 0) ?	(line) :
											(line + 8);
	uint16_t nextIndex  = 0 + strlen(string);
	// Output string
	ST7735_DrawString(0, ui_ConvertedLCDLine, string, us_g_CurrentColor);
	ST7735_DrawString(nextIndex, ui_ConvertedLCDLine, valueString, us_g_CurrentColor);
}


