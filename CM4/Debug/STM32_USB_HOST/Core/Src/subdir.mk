################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_USB_HOST/Core/Src/usbh_core.c \
../STM32_USB_HOST/Core/Src/usbh_ctlreq.c \
../STM32_USB_HOST/Core/Src/usbh_ioreq.c \
../STM32_USB_HOST/Core/Src/usbh_pipes.c 

OBJS += \
./STM32_USB_HOST/Core/Src/usbh_core.o \
./STM32_USB_HOST/Core/Src/usbh_ctlreq.o \
./STM32_USB_HOST/Core/Src/usbh_ioreq.o \
./STM32_USB_HOST/Core/Src/usbh_pipes.o 

C_DEPS += \
./STM32_USB_HOST/Core/Src/usbh_core.d \
./STM32_USB_HOST/Core/Src/usbh_ctlreq.d \
./STM32_USB_HOST/Core/Src/usbh_ioreq.d \
./STM32_USB_HOST/Core/Src/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_USB_HOST/Core/Src/%.o STM32_USB_HOST/Core/Src/%.su STM32_USB_HOST/Core/Src/%.cyclo: ../STM32_USB_HOST/Core/Src/%.c STM32_USB_HOST/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DMETAL_MAX_DEVICE_REGIONS=2 -DRPMSG_BUFFER_SIZE=100 -DSTM32H747xx -DCORE_CM4 -DVIRTIO_SLAVE_ONLY -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -DUSE_USB_HS -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM4/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/CM4/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/App" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/Target" -I"D:/STM32CubeMX/Disco747-xav/CM4/STM32_USB_HOST/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32_USB_HOST-2f-Core-2f-Src

clean-STM32_USB_HOST-2f-Core-2f-Src:
	-$(RM) ./STM32_USB_HOST/Core/Src/usbh_core.cyclo ./STM32_USB_HOST/Core/Src/usbh_core.d ./STM32_USB_HOST/Core/Src/usbh_core.o ./STM32_USB_HOST/Core/Src/usbh_core.su ./STM32_USB_HOST/Core/Src/usbh_ctlreq.cyclo ./STM32_USB_HOST/Core/Src/usbh_ctlreq.d ./STM32_USB_HOST/Core/Src/usbh_ctlreq.o ./STM32_USB_HOST/Core/Src/usbh_ctlreq.su ./STM32_USB_HOST/Core/Src/usbh_ioreq.cyclo ./STM32_USB_HOST/Core/Src/usbh_ioreq.d ./STM32_USB_HOST/Core/Src/usbh_ioreq.o ./STM32_USB_HOST/Core/Src/usbh_ioreq.su ./STM32_USB_HOST/Core/Src/usbh_pipes.cyclo ./STM32_USB_HOST/Core/Src/usbh_pipes.d ./STM32_USB_HOST/Core/Src/usbh_pipes.o ./STM32_USB_HOST/Core/Src/usbh_pipes.su

.PHONY: clean-STM32_USB_HOST-2f-Core-2f-Src

