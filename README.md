# N108
N108 is a full real-time operating system written for the TM4C123 chip, with support for external LCDs. During development, a ST7735 TFT LCD was used for debugging and the driver for that LCD is included.

## Hardware Requirements
This operating system is designed for variations of the ARM TM4C123 chip. However, most of the hardware abstraction layer is stored under OS_Critical/HardwareManager. This module can be very easily swapped for drivers that are supported on other devices. The operating system uses the following hardware tools for its core operation:

1. UART:
  * UART0 is used to interface with an external computer. The operating system features a Unix-like shell that accepts certain commands from the external computer through this port. Runs UART0 w/ interrupt-based data acquisition, and 115200 bps baud.
2. PLL:
  * Internal oscillator. Sets for 80 MHz clock.
3. GPIO:
  * Port A: PA4, PA2, PA5, PA3, PA7 used for LCD.
  * Port F: Entire usable port used for heartbeat purposes.
  * Port D: PD7 used for LCD.

4. SSI:
  * SSI0: Used to interface with the LCD. (See OS_Critical/HardwareManager/LCDDriver/)

## Using The Code
A


## Interfacing
The primary interfacing between the OS (running on the board) is done through the on-board serial UART port. 

