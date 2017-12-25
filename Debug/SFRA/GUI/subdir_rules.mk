################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
SFRA/GUI/SciCommsGui_32bit.obj: ../SFRA/GUI/SciCommsGui_32bit.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_17.6.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=softlib -O1 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_17.6.0.STS/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/common/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/solar/v1.2/IQ/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/headers/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/IQmath/c28/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/SGEN/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/SFRA/IQ/include" --include_path="D:/Projects/Inverter-12V1200W/InverterFWA0/UserInclude" --advice:performance=all --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="SFRA/GUI/SciCommsGui_32bit.d" --obj_directory="SFRA/GUI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


