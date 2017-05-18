			.global aOS_Launch
			.global OS_GetNextThreadToRun
			.global RunningOSInformation
			.global SysTick_Handler
			.global OffsetMode
			.global SVC_Handler
			.global OS_Id
			.global OS_Kill
			.global OS_Sleep
			.global OS_Time
			.global OS_AddThread
			.global ToggleIt

	.align	4
||OSInfoStruct||:	.bits	RunningOSInformation,32
	.align	4

	.align	4
||OSMode||:			.bits	OffsetMode,32
	.align	4



aOS_Launch:
		CPSID 	I
		LDR 		R0, OSInfoStruct
		LDR 		R0, [R0, #0]
		MOV 		R2, R0
		LDR 		SP, [R2]
		POP 		{R4-R11}
		POP 		{R0-R3}
		POP		{R12}
		POP 		{LR}
		POP 		{LR}
		POP 		{R1}
		CPSIE 	I
		BX 		LR



SysTick_Handler:
		CPSID	  I
		PUSH 	  {R4-R11}			; Save the registers
		LDR		  R0, OSInfoStruct	; Load the OSInfoStruct
		LDR		  R0, [R0, #0]     	; R0 now has the address of the current thread running (first element in OSInfo struct)
		MOV		  R1, R0			    ; Move first element (RunPt address) to R1
		STR		  SP, [R1]			; Store SP in the first element of the first element (SP -> [RunPt]) since SP field is first in TCB struct
		PUSH		  {R1-R12, LR}		; About to do a function call so save the registers. We will need to use R0 so dont save it
		BL		  OS_GetNextThreadToRun	; Get the next thread to run (address contained in R0)
		POP		  {R1-R12, LR}		; Pop back everything and restore the registers
		MOV		  R1, R0				; R1 now has the thread to run next, need to store it into RunPt
		LDR		  R0, OSInfoStruct	; Load the OSInfo structs address
		STR		  R1, [R0]			; Replace the first item with the new thread to run
		LDR		  SP, [R1]			; Load the SP with the new thread
		POP		  {R4-R11}
		CPSIE	  I
		BX		  LR


;	User Process Interaction
;	Any User Process Kernel Calls is handled here
SVC_Handler:
		LDR  R12,[SP,#24]
		LDRH R12,[R12,#-2]
		PUSH {LR}
		BL ToggleIt
		POP {LR}
		BIC  R12,#0xFF00
		LDM  SP,{R0-R3}
		CMP  R12, #0			; ID = 0 leads to OS_Id call
		BEQ	 OSIdJmp
		CMP  R12, #1			; ID = 1 leads to OS_Kill
		BEQ	 OSKillJmp
		CMP	 R12, #2
		BEQ	 OSSleepJmp
		CMP	 R12, #3
		BEQ	 OSTimeJmp
		CMP  R12, #4
		BEQ	 OSAddThreadJmp
		B	 Exit			; Unrecognized Kernel Call


OSIdJmp:
		PUSH {R1-R12, LR}
		BL	 OS_Id
		POP	 {R1-R12, LR}
		B	 Exit
OSKillJmp:
		PUSH {R1-R12, LR}
		BL	 OS_Kill
		POP	 {R1-R12, LR}
		B	 Exit
OSSleepJmp:
		PUSH {R1-R12, LR}
		BL	 OS_Sleep
		POP	 {R1-R12, LR}
		B	 Exit
OSTimeJmp:
		PUSH {R1-R12, LR}
		BL	 OS_Time
		POP	 {R1-R12, LR}
		B	 Exit
OSAddThreadJmp:
		PUSH {R1-R12, LR}
		BL	 OS_AddThread
		POP	 {R1-R12, LR}
		B	 Exit
Exit:
		STR	 R0, [SP]		; Save return value
		BX 	 LR



