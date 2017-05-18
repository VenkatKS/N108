

		.global GetStackPointer
		.global SetStackPointer

GetStackPointer:
   	mov r0,sp
   	bx lr


SetStackPointer:
	mov sp, r0
	bx lr



