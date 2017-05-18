/*
 * LCD_Divider.h
 *
 *  Created on: Jan 28, 2016
 *      Author: Venkat
 */

#ifndef LCD_STARTER_LAB2_LCD_DRIVER_ST7735X_H_
#define LCD_STARTER_LAB2_LCD_DRIVER_ST7735X_H_

void ST7735_Message (unsigned int device, unsigned int line, char *string, unsigned long value);
void LCD_Init();
void ST7735_MessageLock(unsigned int device, unsigned int line, char *string, unsigned long value);

#endif /* LCD_STARTER_LAB2_LCD_DRIVER_ST7735X_H_ */
