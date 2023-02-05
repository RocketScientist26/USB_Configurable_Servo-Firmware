################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/System/Drivers/flash.c \
../Core/Src/App/System/Drivers/led.c \
../Core/Src/App/System/Drivers/motor.c \
../Core/Src/App/System/Drivers/pid.c \
../Core/Src/App/System/Drivers/potentiometer.c \
../Core/Src/App/System/Drivers/signal.c \
../Core/Src/App/System/Drivers/usb.c 

OBJS += \
./Core/Src/App/System/Drivers/flash.o \
./Core/Src/App/System/Drivers/led.o \
./Core/Src/App/System/Drivers/motor.o \
./Core/Src/App/System/Drivers/pid.o \
./Core/Src/App/System/Drivers/potentiometer.o \
./Core/Src/App/System/Drivers/signal.o \
./Core/Src/App/System/Drivers/usb.o 

C_DEPS += \
./Core/Src/App/System/Drivers/flash.d \
./Core/Src/App/System/Drivers/led.d \
./Core/Src/App/System/Drivers/motor.d \
./Core/Src/App/System/Drivers/pid.d \
./Core/Src/App/System/Drivers/potentiometer.d \
./Core/Src/App/System/Drivers/signal.d \
./Core/Src/App/System/Drivers/usb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/System/Drivers/%.o Core/Src/App/System/Drivers/%.su: ../Core/Src/App/System/Drivers/%.c Core/Src/App/System/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App-2f-System-2f-Drivers

clean-Core-2f-Src-2f-App-2f-System-2f-Drivers:
	-$(RM) ./Core/Src/App/System/Drivers/flash.d ./Core/Src/App/System/Drivers/flash.o ./Core/Src/App/System/Drivers/flash.su ./Core/Src/App/System/Drivers/led.d ./Core/Src/App/System/Drivers/led.o ./Core/Src/App/System/Drivers/led.su ./Core/Src/App/System/Drivers/motor.d ./Core/Src/App/System/Drivers/motor.o ./Core/Src/App/System/Drivers/motor.su ./Core/Src/App/System/Drivers/pid.d ./Core/Src/App/System/Drivers/pid.o ./Core/Src/App/System/Drivers/pid.su ./Core/Src/App/System/Drivers/potentiometer.d ./Core/Src/App/System/Drivers/potentiometer.o ./Core/Src/App/System/Drivers/potentiometer.su ./Core/Src/App/System/Drivers/signal.d ./Core/Src/App/System/Drivers/signal.o ./Core/Src/App/System/Drivers/signal.su ./Core/Src/App/System/Drivers/usb.d ./Core/Src/App/System/Drivers/usb.o ./Core/Src/App/System/Drivers/usb.su

.PHONY: clean-Core-2f-Src-2f-App-2f-System-2f-Drivers

