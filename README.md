# N108


N108 is a full real-time operating system written for the TM4C123 chip, with support for external LCDs. The kernel of the OS was written from scratch. The OS relies on certain external hardware to function properly, but can be easily modified to be self-sufficient. For a list of external hardware resources needed, please see below.

## Using The Operating System
### Using The OS
The OS is interacted with through a Unix-like Shell. The Shell uses serial communication through the board's UART0 to send commands that you type to the N108 operating system. To interface with the OS, connect your TM4C123 launchpad to your computer through USB and fire-up a serial interface -- such as CoolTerm for Mac or PuTTY for Windows -- and connect to the serial port you're using. From there, you should be able to see the N108 shell's prompt. You can issue commands from this interface.

### Launching Processes
N108 has full process-loading support, so you are able to independently compile and launch your own TM4C123 programs using this OS, without having to re-compile the OS along with your program. To use this feature, compile your independent program using Keil uVision (or any other embedded ARM compiler) and drop the ELF file into a FAT32 formatted SD card. Hook up the SD card to your TM4C123, and run the OS. From the OS's shell, type in 'launch <name>', where '<name>' is the name of the ARM ELF file you have in the SD card. The OS should launch the program and start executing.

### System Calls
The OS supports a single system call: OS_DisplayMessage. The call allows you to print strings to the ST7735 LCD attached to the board. It takes in 4 parameters to specify where on the screen you want the board to display the text. More calls can be easily added by modifying the static const ELFSymbol_t SymbolTable[] array found under OS_Critical/OS.c, and adding a pointer to your new function.


I'm thinking about adding more system calls to this area to allow for easier accesses of TM4C SoC resources (instead of just the external resources). TBD.

## Hardware Requirements
The OS, out of the box, relies on three pieces of external hardware to function normally: an external chip to interface the serial driver with and to issue commands (such as a personal computer), an LCD, and a SD card reader. The LCD is used as an output source (along with the serial port), and the SD card reader is used by the file system and process loader to load and execute external processes. For a list of serial interfacing commands, please look below. I personally used a ST7735 as it allows for SSI-compatible LCD and SD interfacing; the schematic for that is included below as well.

### SoC Hardware Requirements
This operating system is designed for variations of the ARM TM4C123 chip. However, most of the hardware abstraction layer is stored under OS_Critical/HardwareManager. This module can be very easily swapped for drivers that are supported on other devices. The operating system uses the following hardware tools for its core operation:

1. UART:

   * UART0 is used to interface with an external computer. The operating system features a Unix-like shell that accepts certain commands from the external computer through this port. Runs UART0 w/ interrupt-based data acquisition, and 115200 bps baud.

2. PLL:
   * Internal oscillator:
     * Sets for 80 MHz clock.
3. GPIO:
    * Ports A, F, D are used for various purposes.
      * Port A: PA4, PA2, PA5, PA3, PA7 used for LCD.
      * Port F: Entire usable port used for heartbeat purposes.
      * Port D: PD7 used for LCD.

4. SSI:
    * SSI_0:
      * Used to interface with the LCD. (See OS_Critical/HardwareManager/LCDDriver/)
      * This is also used to interface with the ST7735's on-board SD reader. This is helpful         when loading files onto the operating system's FAT32 filesystem.

## Using The Code
The code is designed to be used with Texas Instruments Code Composer Studio. Just clone the repository, and import the project into your CCS workspace. The project should build off the bat, with no additional work needed.
### Flashing The Board
This operating system will flash out-of-the-box onto a TI TM4C123 Launchpad.
### Hardware Schematics
The OS uses an external ST7735 LCD for two things: as a LCD and as an external SD card reader for its filesystem. My OS was also used for other purposes -- such as interfacing with sensors -- so please ignore any extraneous connections in the second image. What's important is getting the ST7735 connected.

Here are the schematics for how to hook up this to your launchpad:
![alt text](https://github.com/VenkatKS/N108/raw/master/Schematics/st7735_in.png "Connections going into the ST7735")
![Launchpad_Out](https://github.com/VenkatKS/N108/raw/master/Schematics/launchpad_out.png "Connections going out of the Launchpad")

## Interfacing
The primary interfacing between the OS (running on the board) is done through the on-board serial UART port. You can use any terminal program on your computer -- such as CoolEdge on Mac or Putty on Windows -- to send these commands over to the OS interpreter once boot-up is complete.

Out of the box, the operating system supports the following commands, as seen in Shell/shell.c:

```
char* 			commandFormat[]		=
                      {
                        "help",
                        "runproc <processname>"
                      }
```

Very barebones, but the idea here is to add your own commands and interface with the rest of the operating system. **Help** basically prints out this same list, while **runproc** loads an external process (stored in the SD card inserted into the ST7735 breakout board) and starts executing that program. During idle, only the interpreter runs.

