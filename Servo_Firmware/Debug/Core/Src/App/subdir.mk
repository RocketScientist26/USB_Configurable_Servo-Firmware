################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/app.c \
../Core/Src/App/flash.c \
../Core/Src/App/general.c \
../Core/Src/App/interrupts.c \
../Core/Src/App/led.c \
../Core/Src/App/motor.c \
../Core/Src/App/pid.c \
../Core/Src/App/potentiometer.c \
../Core/Src/App/settings.c \
../Core/Src/App/signal.c \
../Core/Src/App/usb.c 

OBJS += \
./Core/Src/App/app.o \
./Core/Src/App/flash.o \
./Core/Src/App/general.o \
./Core/Src/App/interrupts.o \
./Core/Src/App/led.o \
./Core/Src/App/motor.o \
./Core/Src/App/pid.o \
./Core/Src/App/potentiometer.o \
./Core/Src/App/settings.o \
./Core/Src/App/signal.o \
./Core/Src/App/usb.o 

C_DEPS += \
./Core/Src/App/app.d \
./Core/Src/App/flash.d \
./Core/Src/App/general.d \
./Core/Src/App/interrupts.d \
./Core/Src/App/led.d \
./Core/Src/App/motor.d \
./Core/Src/App/pid.d \
./Core/Src/App/potentiometer.d \
./Core/Src/App/settings.d \
./Core/Src/App/signal.d \
./Core/Src/App/usb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

