################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ST7735/st7735.c 

OBJS += \
./Drivers/ST7735/st7735.o 

C_DEPS += \
./Drivers/ST7735/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ST7735/st7735.o: ../Drivers/ST7735/st7735.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_FULL_LL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"../Drivers/ST7735" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/ST7735/st7735.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

