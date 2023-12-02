################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery.c \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery_audio.c \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery_bus.c 

OBJS += \
./Drivers/BSP/stm32h747i_discovery.o \
./Drivers/BSP/stm32h747i_discovery_audio.o \
./Drivers/BSP/stm32h747i_discovery_bus.o 

C_DEPS += \
./Drivers/BSP/stm32h747i_discovery.d \
./Drivers/BSP/stm32h747i_discovery_audio.d \
./Drivers/BSP/stm32h747i_discovery_bus.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/stm32h747i_discovery.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/stm32h747i_discovery_audio.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery_audio.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/stm32h747i_discovery_bus.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/stm32h747i_discovery_bus.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/stm32h747i_discovery.cyclo ./Drivers/BSP/stm32h747i_discovery.d ./Drivers/BSP/stm32h747i_discovery.o ./Drivers/BSP/stm32h747i_discovery.su ./Drivers/BSP/stm32h747i_discovery_audio.cyclo ./Drivers/BSP/stm32h747i_discovery_audio.d ./Drivers/BSP/stm32h747i_discovery_audio.o ./Drivers/BSP/stm32h747i_discovery_audio.su ./Drivers/BSP/stm32h747i_discovery_bus.cyclo ./Drivers/BSP/stm32h747i_discovery_bus.d ./Drivers/BSP/stm32h747i_discovery_bus.o ./Drivers/BSP/stm32h747i_discovery_bus.su

.PHONY: clean-Drivers-2f-BSP

