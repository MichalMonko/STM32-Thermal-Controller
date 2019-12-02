################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuration.c \
../src/ds18b20.c \
../src/i2c.c \
../src/keyboard.c \
../src/lcd.c \
../src/main.c \
../src/mainmenu.c \
../src/one_wire.c \
../src/radio2401l.c \
../src/spi_driver.c \
../src/syscalls.c \
../src/system_stm32f10x.c \
../src/timer.c 

OBJS += \
./src/configuration.o \
./src/ds18b20.o \
./src/i2c.o \
./src/keyboard.o \
./src/lcd.o \
./src/main.o \
./src/mainmenu.o \
./src/one_wire.o \
./src/radio2401l.o \
./src/spi_driver.o \
./src/syscalls.o \
./src/system_stm32f10x.o \
./src/timer.o 

C_DEPS += \
./src/configuration.d \
./src/ds18b20.d \
./src/i2c.d \
./src/keyboard.d \
./src/lcd.d \
./src/main.d \
./src/mainmenu.d \
./src/one_wire.d \
./src/radio2401l.d \
./src/spi_driver.d \
./src/syscalls.d \
./src/system_stm32f10x.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/monko/git/driver/StdPeriph_Driver/inc" -I"/home/monko/git/driver/inc" -I"/home/monko/git/driver/CMSIS/device" -I"/home/monko/git/driver/CMSIS/core" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


