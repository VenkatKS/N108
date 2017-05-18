

	.global IntTerm
	.global PrevError
	.global	PID_stm32

	.align	4
||IntTermint||:	.bits	IntTerm,32
	.align	4

	.align	4
||PrevErrorint||:	.bits	PrevError,32
	.align	4

PID_stm32:

  PUSH {R4, R5, R9}

  LDR 	R12, IntTermint
  LDR 	R9, PrevErrorint

  LDRH 	R3, [R1, #0]  		; Load Kp
  LDRH 	R2, [R1, #2]  		; Load Ki
  LDRH 	R1, [R1, #4]  		; Load Kd and destroy Coeff
  LDRH 	R5, [R12, #0]  		; Last Integral Term
  LDRH 	R12, [R9, #0]  		; Previous Error

  MLA 	R5, R2, R0, R5   	; IntTerm += Ki*error
  MLA 	R4, R3, R0, R5      	; Output = (Kp * error) + InTerm
  SUBS 	R12, R0, R12    ; PrevErr now holds DeltaError = Error - PrevError
  MLA 	R2, R1, R12, R4  ; Output += Kd * DeltaError

  LDR 	R12, IntTermint
  STRH 	R5, [R12, #0]       	; Write back InTerm
  STRH 	R0, [R9, #0]         	; Write back PrevError

  MOV 	R0, R2
  UXTH 	R0, R0
  POP 	{R4, R5, R9}
  BX 	LR

.end
