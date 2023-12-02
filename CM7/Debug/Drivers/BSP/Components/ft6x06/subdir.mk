################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/ft6x06/ft6x06.c \
D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/ft6x06/ft6x06_reg.c 

OBJS += \
./Drivers/BSP/Components/ft6x06/ft6x06.o \
./Drivers/BSP/Components/ft6x06/ft6x06_reg.o 

C_DEPS += \
./Drivers/BSP/Components/ft6x06/ft6x06.d \
./Drivers/BSP/Components/ft6x06/ft6x06_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ft6x06/ft6x06.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/ft6x06/ft6x06.c Drivers/BSP/Components/ft6x06/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/ft6x06/ft6x06_reg.o: D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/ft6x06/ft6x06_reg.c Drivers/BSP/Components/ft6x06/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-ft6x06

clean-Drivers-2f-BSP-2f-Components-2f-ft6x06:
	-$(RM) ./Drivers/BSP/Components/ft6x06/ft6x06.cyclo ./Drivers/BSP/Components/ft6x06/ft6x06.d ./Drivers/BSP/Components/ft6x06/ft6x06.o ./Drivers/BSP/Components/ft6x06/ft6x06.su ./Drivers/BSP/Components/ft6x06/ft6x06_reg.cyclo ./Drivers/BSP/Components/ft6x06/ft6x06_reg.d ./Drivers/BSP/Components/ft6x06/ft6x06_reg.o ./Drivers/BSP/Components/ft6x06/ft6x06_reg.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-ft6x06

