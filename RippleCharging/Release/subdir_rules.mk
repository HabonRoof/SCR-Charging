################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O2 --include_path="C:/Users/Johnson/Documents/TI_workspace/RippleCharging/RippleCharging" --include_path="C:/ti/c2000/C2000Ware_3_04_00_00/device_support/f28004x/common/include" --include_path="C:/ti/c2000/C2000Ware_3_04_00_00/device_support/f28004x/headers/include" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O2 --include_path="C:/Users/Johnson/Documents/TI_workspace/RippleCharging/RippleCharging" --include_path="C:/ti/c2000/C2000Ware_3_04_00_00/device_support/f28004x/common/include" --include_path="C:/ti/c2000/C2000Ware_3_04_00_00/device_support/f28004x/headers/include" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


