################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/nt35510/nt35510.c \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/nt35510/nt35510_reg.c 

OBJS += \
./Drivers/BSP/Components/nt35510/nt35510.o \
./Drivers/BSP/Components/nt35510/nt35510_reg.o 

C_DEPS += \
./Drivers/BSP/Components/nt35510/nt35510.d \
./Drivers/BSP/Components/nt35510/nt35510_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/nt35510/nt35510.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/nt35510/nt35510.c Drivers/BSP/Components/nt35510/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/nt35510/nt35510_reg.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/nt35510/nt35510_reg.c Drivers/BSP/Components/nt35510/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-nt35510

clean-Drivers-2f-BSP-2f-Components-2f-nt35510:
	-$(RM) ./Drivers/BSP/Components/nt35510/nt35510.cyclo ./Drivers/BSP/Components/nt35510/nt35510.d ./Drivers/BSP/Components/nt35510/nt35510.o ./Drivers/BSP/Components/nt35510/nt35510.su ./Drivers/BSP/Components/nt35510/nt35510_reg.cyclo ./Drivers/BSP/Components/nt35510/nt35510_reg.d ./Drivers/BSP/Components/nt35510/nt35510_reg.o ./Drivers/BSP/Components/nt35510/nt35510_reg.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-nt35510

