################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CC1350_LAUNCHXL.obj: ../CC1350_LAUNCHXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="CC1350_LAUNCHXL.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Gpio_function.obj: ../Gpio_function.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Gpio_function.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Timer_function.obj: ../Timer_function.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Timer_function.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ccfg.obj: ../ccfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="ccfg.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main_tirtos.obj: ../main_tirtos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main_tirtos.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdi.obj: ../sdi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="sdi.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdi12_top.obj: ../sdi12_top.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/source/ti/devices/cc13x0/inc" --include_path="C:/Users/Public/Documents/SDI-12v1" --include_path="C:/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="sdi12_top.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


