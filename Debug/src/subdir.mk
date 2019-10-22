################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuration.c \
../src/i2c.c \
../src/lcd.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f10x.c \
../src/timer.c 

OBJS += \
./src/configuration.o \
./src/i2c.o \
./src/lcd.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f10x.o \
./src/timer.o 

C_DEPS += \
./src/configuration.d \
./src/i2c.d \
./src/lcd.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f10x.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/warchlak/stm32workspace/bluepillsample/StdPeriph_Driver/inc" -I"/home/warchlak/stm32workspace/bluepillsample/inc" -I"/home/warchlak/stm32workspace/bluepillsample/CMSIS/device" -I"/home/warchlak/stm32workspace/bluepillsample/CMSIS/core" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


