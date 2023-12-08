################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.c 

OBJS += \
./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.o 

C_DEPS += \
./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/OpenAMP/open-amp/lib/remoteproc/%.o Third_Party/OpenAMP/open-amp/lib/remoteproc/%.su Third_Party/OpenAMP/open-amp/lib/remoteproc/%.cyclo: ../Third_Party/OpenAMP/open-amp/lib/remoteproc/%.c Third_Party/OpenAMP/open-amp/lib/remoteproc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DMETAL_MAX_DEVICE_REGIONS=2 -DRPMSG_BUFFER_SIZE=100 -DSTM32H747xx -DCORE_CM4 -DVIRTIO_SLAVE_ONLY -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -DUSE_USB_HS -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM4/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/CM4/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/App" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/Target" -I"D:/STM32CubeMX/Disco747-xav/CM4/STM32_USB_HOST/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-remoteproc

clean-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-remoteproc:
	-$(RM) ./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.cyclo ./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.d ./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.o ./Third_Party/OpenAMP/open-amp/lib/remoteproc/remoteproc_virtio.su

.PHONY: clean-Third_Party-2f-OpenAMP-2f-open-2d-amp-2f-lib-2f-remoteproc
