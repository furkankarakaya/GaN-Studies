################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
sources/F2837xD_CodeStartBranch.obj: ../sources/F2837xD_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_CodeStartBranch.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_CpuTimers.obj: ../sources/F2837xD_CpuTimers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_CpuTimers.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_DefaultISR.obj: ../sources/F2837xD_DefaultISR.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_DefaultISR.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Dma.obj: ../sources/F2837xD_Dma.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Dma.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_ECap.obj: ../sources/F2837xD_ECap.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_ECap.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_EPwm.obj: ../sources/F2837xD_EPwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_EPwm.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_EQep.obj: ../sources/F2837xD_EQep.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_EQep.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_GlobalVariableDefs.obj: ../sources/F2837xD_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_GlobalVariableDefs.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Gpio.obj: ../sources/F2837xD_Gpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Gpio.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_I2C.obj: ../sources/F2837xD_I2C.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_I2C.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Ipc.obj: ../sources/F2837xD_Ipc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Ipc.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Mcbsp.obj: ../sources/F2837xD_Mcbsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Mcbsp.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Pbist.obj: ../sources/F2837xD_Pbist.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Pbist.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_PieCtrl.obj: ../sources/F2837xD_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_PieCtrl.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_PieVect.obj: ../sources/F2837xD_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_PieVect.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Sci.obj: ../sources/F2837xD_Sci.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Sci.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Spi.obj: ../sources/F2837xD_Spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Spi.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_SysCtrl.obj: ../sources/F2837xD_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_SysCtrl.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_TempSensorConv.obj: ../sources/F2837xD_TempSensorConv.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_TempSensorConv.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_Upp.obj: ../sources/F2837xD_Upp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_Upp.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_sci_io.obj: ../sources/F2837xD_sci_io.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_sci_io.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_sdfm_drivers.obj: ../sources/F2837xD_sdfm_drivers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_sdfm_drivers.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_struct.obj: ../sources/F2837xD_struct.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_struct.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sources/F2837xD_usDelay.obj: ../sources/F2837xD_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="D:/GitHub Repositories/GaN-Studies/Buck Converter/Buck Code/control_board_v0.2_dock" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xD/v100/F2837xD_common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sources/F2837xD_usDelay.d_raw" --obj_directory="sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


