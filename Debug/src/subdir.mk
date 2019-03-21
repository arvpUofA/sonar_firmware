################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/clocks.c \
../src/dma.c \
../src/main.c \
../src/stm32f3xx_it.c \
../src/syscalls.c \
../src/system_stm32f3xx.c \
../src/timers.c 

OBJS += \
./src/adc.o \
./src/clocks.o \
./src/dma.o \
./src/main.o \
./src/stm32f3xx_it.o \
./src/syscalls.o \
./src/system_stm32f3xx.o \
./src/timers.o 

C_DEPS += \
./src/adc.d \
./src/clocks.d \
./src/dma.d \
./src/main.d \
./src/stm32f3xx_it.d \
./src/syscalls.d \
./src/system_stm32f3xx.d \
./src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F3 -DSTM32F30 -DSTM32F303CBTx -DDEBUG -DSTM32F303xC -DUSE_HAL_DRIVER -I"/home/isthatme/Ac6/Workspace/sonar_CB/HAL_Driver/Inc/Legacy" -I"/home/isthatme/Ac6/Workspace/sonar_CB/inc" -I"/home/isthatme/Ac6/Workspace/sonar_CB/CMSIS/device" -I"/home/isthatme/Ac6/Workspace/sonar_CB/CMSIS/core" -I"/home/isthatme/Ac6/Workspace/sonar_CB/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


