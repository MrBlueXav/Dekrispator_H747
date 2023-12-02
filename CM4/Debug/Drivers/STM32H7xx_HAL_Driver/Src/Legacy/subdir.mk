################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.c \
D:/STM32CubeMX/Disco747-xav/Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.c 

OBJS += \
./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.o \
./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.o 

C_DEPS += \
./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.d \
./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.o: D:/STM32CubeMX/Disco747-xav/Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.c Drivers/STM32H7xx_HAL_Driver/Src/Legacy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.o: D:/STM32CubeMX/Disco747-xav/Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.c Drivers/STM32H7xx_HAL_Driver/Src/Legacy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src-2f-Legacy

clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src-2f-Legacy:
	-$(RM) ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.d ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.o ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth.su ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/Legacy/stm32h7xx_hal_eth_ex.su

.PHONY: clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src-2f-Legacy

