################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.c \
D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.c 

OBJS += \
./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.o \
./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.o 

C_DEPS += \
./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.d \
./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.o: D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.c Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DMETAL_MAX_DEVICE_REGIONS=2 -DRPMSG_BUFFER_SIZE=128 -DSTM32H747xx -DCORE_CM4 -DVIRTIO_SLAVE_ONLY -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/Common" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/Common" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/Fonts" -I"D:/STM32CubeMX/Disco747-xav/Drivers/lcd" -I"D:/STM32CubeMX/Disco747-xav/Binn" -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.o: D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.c Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DMETAL_MAX_DEVICE_REGIONS=2 -DRPMSG_BUFFER_SIZE=128 -DSTM32H747xx -DCORE_CM4 -DVIRTIO_SLAVE_ONLY -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/Common" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/Common" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/Fonts" -I"D:/STM32CubeMX/Disco747-xav/Drivers/lcd" -I"D:/STM32CubeMX/Disco747-xav/Binn" -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-rpmsg

clean-Middlewares-2f-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-rpmsg:
	-$(RM) ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.cyclo ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.d ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.o ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg.su ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.cyclo ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.d ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.o ./Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-rpmsg

