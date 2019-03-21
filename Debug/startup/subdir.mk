################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/isthatme/Ac6/Workspace/sonar_CB/HAL_Driver/Inc/Legacy" -I"/home/isthatme/Ac6/Workspace/sonar_CB/inc" -I"/home/isthatme/Ac6/Workspace/sonar_CB/CMSIS/device" -I"/home/isthatme/Ac6/Workspace/sonar_CB/CMSIS/core" -I"/home/isthatme/Ac6/Workspace/sonar_CB/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


