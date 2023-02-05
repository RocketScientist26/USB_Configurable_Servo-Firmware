################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/app.c \
../Core/Src/App/callbacks.c \
../Core/Src/App/settings.c 

OBJS += \
./Core/Src/App/app.o \
./Core/Src/App/callbacks.o \
./Core/Src/App/settings.o 

C_DEPS += \
./Core/Src/App/app.d \
./Core/Src/App/callbacks.d \
./Core/Src/App/settings.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/app.d ./Core/Src/App/app.o ./Core/Src/App/app.su ./Core/Src/App/callbacks.d ./Core/Src/App/callbacks.o ./Core/Src/App/callbacks.su ./Core/Src/App/settings.d ./Core/Src/App/settings.o ./Core/Src/App/settings.su

.PHONY: clean-Core-2f-Src-2f-App

