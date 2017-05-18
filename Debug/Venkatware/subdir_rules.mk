################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Venkatware/PID_stm32.obj: ../Venkatware/PID_stm32.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="Venkatware/PID_stm32.pp" --obj_directory="Venkatware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Venkatware/aVenkatware.obj: ../Venkatware/aVenkatware.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="Venkatware/aVenkatware.pp" --obj_directory="Venkatware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Venkatware/fft.obj: ../Venkatware/fft.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="Venkatware/fft.pp" --obj_directory="Venkatware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Venkatware/venkatlib.obj: ../Venkatware/venkatlib.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" --include_path="/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GE6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="Venkatware/venkatlib.pp" --obj_directory="Venkatware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


