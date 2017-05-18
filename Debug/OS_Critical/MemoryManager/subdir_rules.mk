################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
OS_Critical/MemoryManager/OS_Allocation.obj: ../OS_Critical/MemoryManager/OS_Allocation.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="OS_Critical/MemoryManager/OS_Allocation.pp" --obj_directory="OS_Critical/MemoryManager" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

OS_Critical/MemoryManager/heap.obj: ../OS_Critical/MemoryManager/heap.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="OS_Critical/MemoryManager/heap.pp" --obj_directory="OS_Critical/MemoryManager" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


