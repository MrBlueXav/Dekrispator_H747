################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OPENAMP/mbox_hsem.c \
../OPENAMP/openamp.c \
../OPENAMP/rsc_table.c 

OBJS += \
./OPENAMP/mbox_hsem.o \
./OPENAMP/openamp.o \
./OPENAMP/rsc_table.o 

C_DEPS += \
./OPENAMP/mbox_hsem.d \
./OPENAMP/openamp.d \
./OPENAMP/rsc_table.d 


# Each subdirectory must supply rules for building sources it contributes
OPENAMP/%.o OPENAMP/%.su OPENAMP/%.cyclo: ../OPENAMP/%.c OPENAMP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DMETAL_MAX_DEVICE_REGIONS=2 -DRPMSG_BUFFER_SIZE=100 -DSTM32H747xx -DCORE_CM4 -DVIRTIO_SLAVE_ONLY -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/OPENAMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-OPENAMP

clean-OPENAMP:
	-$(RM) ./OPENAMP/mbox_hsem.cyclo ./OPENAMP/mbox_hsem.d ./OPENAMP/mbox_hsem.o ./OPENAMP/mbox_hsem.su ./OPENAMP/openamp.cyclo ./OPENAMP/openamp.d ./OPENAMP/openamp.o ./OPENAMP/openamp.su ./OPENAMP/rsc_table.cyclo ./OPENAMP/rsc_table.d ./OPENAMP/rsc_table.o ./OPENAMP/rsc_table.su

.PHONY: clean-OPENAMP

