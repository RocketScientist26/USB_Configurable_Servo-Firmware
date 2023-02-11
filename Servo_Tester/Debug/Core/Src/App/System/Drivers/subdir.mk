################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/System/Drivers/led.c \
../Core/Src/App/System/Drivers/lock.c \
../Core/Src/App/System/Drivers/potentiometer.c \
../Core/Src/App/System/Drivers/pwm.c 

OBJS += \
./Core/Src/App/System/Drivers/led.o \
./Core/Src/App/System/Drivers/lock.o \
./Core/Src/App/System/Drivers/potentiometer.o \
./Core/Src/App/System/Drivers/pwm.o 

C_DEPS += \
./Core/Src/App/System/Drivers/led.d \
./Core/Src/App/System/Drivers/lock.d \
./Core/Src/App/System/Drivers/potentiometer.d \
./Core/Src/App/System/Drivers/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/System/Drivers/%.o Core/Src/App/System/Drivers/%.su: ../Core/Src/App/System/Drivers/%.c Core/Src/App/System/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App-2f-System-2f-Drivers

clean-Core-2f-Src-2f-App-2f-System-2f-Drivers:
	-$(RM) ./Core/Src/App/System/Drivers/led.d ./Core/Src/App/System/Drivers/led.o ./Core/Src/App/System/Drivers/led.su ./Core/Src/App/System/Drivers/lock.d ./Core/Src/App/System/Drivers/lock.o ./Core/Src/App/System/Drivers/lock.su ./Core/Src/App/System/Drivers/potentiometer.d ./Core/Src/App/System/Drivers/potentiometer.o ./Core/Src/App/System/Drivers/potentiometer.su ./Core/Src/App/System/Drivers/pwm.d ./Core/Src/App/System/Drivers/pwm.o ./Core/Src/App/System/Drivers/pwm.su

.PHONY: clean-Core-2f-Src-2f-App-2f-System-2f-Drivers

