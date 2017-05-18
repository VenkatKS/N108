Repository features code for a full-fledged operating system, written from scratch, that is designed to run on TM4C123 chips. Named myOS, the operating system features the following high-level features:

* FAT32 File System
* Full Process Loading Support (external program needs to be compiled using Keil’s Position Independent Compilation/Linking)
* Priority Scheduler
* SD Card Drivers (Allows for externally compiled programs to be loaded from SD card)
* UART Terminal Support/Parsing (Supply OS commands to TM4C123 using the UART)

To use the code, please import it into your Code Composer Studio (TI) workspace.

For detailed explanation of the code and design choices, please navigate to venkatks.com. There are PDFs within the Projects section that details the design of the OS (including a link to a BitBucket repo that contains a variation of my OS that utilizes the FileSystem that I designed).
