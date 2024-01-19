################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_HOST/Target/subdir.mk
-include USB_HOST/App/subdir.mk
-include OPENAMP/subdir.mk
-include Middlewares/Third_Party/OpenAMP/open-amp/lib/virtio/subdir.mk
-include Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/subdir.mk
-include Middlewares/Third_Party/OpenAMP/open-amp/lib/remoteproc/subdir.mk
-include Middlewares/Third_Party/OpenAMP/libmetal/lib/system/generic/cortexm/subdir.mk
-include Middlewares/Third_Party/OpenAMP/libmetal/lib/system/generic/subdir.mk
-include Middlewares/Third_Party/OpenAMP/libmetal/lib/subdir.mk
-include Middlewares/ST/STM32_USB_Host_Library/Core/Src/subdir.mk
-include Drivers/lcd/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/Components/otm8009a/subdir.mk
-include Drivers/BSP/Components/nt35510/subdir.mk
-include Drivers/BSP/Components/mt25tl01g/subdir.mk
-include Drivers/BSP/Components/is42s32800j/subdir.mk
-include Drivers/BSP/Components/ft6x06/subdir.mk
-include Drivers/BSP/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Common/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Dekrispator_H747_CM4
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Dekrispator_H747_CM4.elf \

MAP_FILES += \
Dekrispator_H747_CM4.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Dekrispator_H747_CM4.list \

OBJCOPY_HEX += \
Dekrispator_H747_CM4.hex \

OBJCOPY_BIN += \
Dekrispator_H747_CM4.bin \


# All Target
all: main-build

# Main-build Target
main-build: Dekrispator_H747_CM4.elf secondary-outputs

# Tool invocations
Dekrispator_H747_CM4.elf Dekrispator_H747_CM4.map: $(OBJS) $(USER_OBJS) D:\STM32CubeMX\Disco747-xav\CM4\STM32H747XIHX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Dekrispator_H747_CM4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\STM32CubeMX\Disco747-xav\CM4\STM32H747XIHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Dekrispator_H747_CM4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Dekrispator_H747_CM4.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Dekrispator_H747_CM4.list"
	@echo 'Finished building: $@'
	@echo ' '

Dekrispator_H747_CM4.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Dekrispator_H747_CM4.hex"
	@echo 'Finished building: $@'
	@echo ' '

Dekrispator_H747_CM4.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Dekrispator_H747_CM4.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Dekrispator_H747_CM4.bin Dekrispator_H747_CM4.elf Dekrispator_H747_CM4.hex Dekrispator_H747_CM4.list Dekrispator_H747_CM4.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
