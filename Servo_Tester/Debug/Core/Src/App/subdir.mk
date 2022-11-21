################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/app.c \
../Core/Src/App/interrupts.c \
../Core/Src/App/led.c \
../Core/Src/App/lock.c \
../Core/Src/App/potentiometer.c \
../Core/Src/App/pwm.c 

OBJS += \
./Core/Src/App/app.o \
./Core/Src/App/interrupts.o \
./Core/Src/App/led.o \
./Core/Src/App/lock.o \
./Core/Src/App/potentiometer.o \
./Core/Src/App/pwm.o 

C_DEPS += \
./Core/Src/App/app.d \
./Core/Src/App/interrupts.d \
./Core/Src/App/led.d \
./Core/Src/App/lock.d \
./Core/Src/App/potentiometer.d \
./Core/Src/App/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/app.d ./Core/Src/App/app.o ./Core/Src/App/app.su ./Core/Src/App/interrupts.d ./Core/Src/App/interrupts.o ./Core/Src/App/interrupts.su ./Core/Src/App/led.d ./Core/Src/App/led.o ./Core/Src/App/led.su ./Core/Src/App/lock.d ./Core/Src/App/lock.o ./Core/Src/App/lock.su ./Core/Src/App/potentiometer.d ./Core/Src/App/potentiometer.o ./Core/Src/App/potentiometer.su ./Core/Src/App/pwm.d ./Core/Src/App/pwm.o ./Core/Src/App/pwm.su

.PHONY: clean-Core-2f-Src-2f-App

