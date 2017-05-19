# N108
N108 is a full real-time operating system written for the TM4C123 chip.

## Hardware Requirements
This operating system is designed for variations of the ARM TM4C123 chip. The operating system uses the following hardware tools for its operation:
  * UART0: This UART is used to interface with an external computer. The operating system features a Unix-like shell that accepts certain commands from the external computer through this port. Runs UART0 w/ interrupt-based data acquisition, and 115200 bps baud.
  * PLL: Internal oscillator. Sets for 80 MHz clock.
  * 

## Using The Code
A


## Interfacing
The primary interfacing between the OS (running on the board) is done through the on-board serial UART port. 

