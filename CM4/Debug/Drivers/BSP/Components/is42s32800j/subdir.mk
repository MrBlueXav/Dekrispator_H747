################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/is42s32800j/is42s32800j.c 

OBJS += \
./Drivers/BSP/Components/is42s32800j/is42s32800j.o 

C_DEPS += \
./Drivers/BSP/Components/is42s32800j/is42s32800j.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/is42s32800j/is42s32800j.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/is42s32800j/is42s32800j.c Drivers/BSP/Components/is42s32800j/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j:
	-$(RM) ./Drivers/BSP/Components/is42s32800j/is42s32800j.cyclo ./Drivers/BSP/Components/is42s32800j/is42s32800j.d ./Drivers/BSP/Components/is42s32800j/is42s32800j.o ./Drivers/BSP/Components/is42s32800j/is42s32800j.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

