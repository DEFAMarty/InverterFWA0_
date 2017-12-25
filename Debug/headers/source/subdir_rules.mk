################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
headers/source/DSP2803x_GlobalVariableDefs.obj: ../headers/source/DSP2803x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/17.6.0/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=softlib -O1 --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/solar/v1.2/IQ/include" --include_path="C:/ti/17.6.0/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/common/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/common/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/solar/v1.2/IQ/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/headers/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/IQmath/c28/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/SGEN/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/SFRA/IQ/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/UserInclude" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/headers/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/IQmath/c28/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/SGEN/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/SFRA/IQ/include" --include_path="R:/08 - R&D/12 - Marty Ma/Firmware/InverterFWA0/UserInclude" --advice:performance=all --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="headers/source/DSP2803x_GlobalVariableDefs.d" --obj_directory="headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


