# N108


N108 is a full real-time operating system written for the TM4C123 chip, with support for external LCDs. During development, a ST7735 TFT LCD was used for debugging and the driver for that LCD is included.

## Hardware Requirements
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
The OS uses an external ST7735 LCD for two things: as a LCD and as an external SD card reader for its filesystem. Here are the schematics for how to hook up this to your launchpad:
![alt text](https://github.com/VenkatKS/N108/raw/master/Schematics/st7735_in.png "Connections going into the ST7735")
![Launchpad_Out](https://github.com/VenkatKS/N108/raw/master/Schematics/launchpad_out.png "Connections going out of the Launchpad")
My OS was also used for other purposes -- such as interfacing with sensors -- so please ignore any extraneous connections in the second image. What's important is getting the ST7735 connected.
## Interfacing
The primary interfacing between the OS (running on the board) is done through the on-board serial UART port.
