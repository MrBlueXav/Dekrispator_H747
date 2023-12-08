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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DDEBUG -DMETAL_MAX_DEVICE_REGIONS=2 -DVIRTIO_MASTER_ONLY -DRPMSG_BUFFER_SIZE=100 -DSTM32H747xx -DCORE_CM7 -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -I"D:/STM32CubeMX/Disco747-xav/CM7/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/App" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j:
	-$(RM) ./Drivers/BSP/Components/is42s32800j/is42s32800j.cyclo ./Drivers/BSP/Components/is42s32800j/is42s32800j.d ./Drivers/BSP/Components/is42s32800j/is42s32800j.o ./Drivers/BSP/Components/is42s32800j/is42s32800j.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

