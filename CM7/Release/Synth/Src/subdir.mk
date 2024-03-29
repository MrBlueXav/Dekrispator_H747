################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Synth/Src/MIDI_application.c \
../Synth/Src/additive_osc.c \
../Synth/Src/adsr.c \
../Synth/Src/audio_conf.c \
../Synth/Src/blepvco.c \
../Synth/Src/chorusFD.c \
../Synth/Src/delay.c \
../Synth/Src/drift_3osc.c \
../Synth/Src/drifter.c \
../Synth/Src/math_tools.c \
../Synth/Src/metronome.c \
../Synth/Src/minblep_tables.c \
../Synth/Src/notesTables.c \
../Synth/Src/osc.c \
../Synth/Src/phaser.c \
../Synth/Src/random.c \
../Synth/Src/rational_ratios.c \
../Synth/Src/resonantFilter.c \
../Synth/Src/sequencer.c \
../Synth/Src/sinetable.c \
../Synth/Src/soundgen.c 

OBJS += \
./Synth/Src/MIDI_application.o \
./Synth/Src/additive_osc.o \
./Synth/Src/adsr.o \
./Synth/Src/audio_conf.o \
./Synth/Src/blepvco.o \
./Synth/Src/chorusFD.o \
./Synth/Src/delay.o \
./Synth/Src/drift_3osc.o \
./Synth/Src/drifter.o \
./Synth/Src/math_tools.o \
./Synth/Src/metronome.o \
./Synth/Src/minblep_tables.o \
./Synth/Src/notesTables.o \
./Synth/Src/osc.o \
./Synth/Src/phaser.o \
./Synth/Src/random.o \
./Synth/Src/rational_ratios.o \
./Synth/Src/resonantFilter.o \
./Synth/Src/sequencer.o \
./Synth/Src/sinetable.o \
./Synth/Src/soundgen.o 

C_DEPS += \
./Synth/Src/MIDI_application.d \
./Synth/Src/additive_osc.d \
./Synth/Src/adsr.d \
./Synth/Src/audio_conf.d \
./Synth/Src/blepvco.d \
./Synth/Src/chorusFD.d \
./Synth/Src/delay.d \
./Synth/Src/drift_3osc.d \
./Synth/Src/drifter.d \
./Synth/Src/math_tools.d \
./Synth/Src/metronome.d \
./Synth/Src/minblep_tables.d \
./Synth/Src/notesTables.d \
./Synth/Src/osc.d \
./Synth/Src/phaser.d \
./Synth/Src/random.d \
./Synth/Src/rational_ratios.d \
./Synth/Src/resonantFilter.d \
./Synth/Src/sequencer.d \
./Synth/Src/sinetable.d \
./Synth/Src/soundgen.d 


# Each subdirectory must supply rules for building sources it contributes
Synth/Src/%.o Synth/Src/%.su Synth/Src/%.cyclo: ../Synth/Src/%.c Synth/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DMETAL_MAX_DEVICE_REGIONS=2 -DVIRTIO_MASTER_ONLY -DRPMSG_BUFFER_SIZE=128 -DSTM32H747xx -DCORE_CM7 -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"D:/STM32CubeMX/Disco747-xav/CM7/Synth/Inc" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components" -I"D:/STM32CubeMX/Disco747-xav/CM7/OPENAMP" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/open-amp/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/Third_Party/OpenAMP/libmetal/lib/include" -I"D:/STM32CubeMX/Disco747-xav/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"D:/STM32CubeMX/Disco747-xav/CM4/USB_HOST/App" -I"D:/STM32CubeMX/Disco747-xav/Common" -I"D:/STM32CubeMX/Disco747-xav/Drivers/BSP/Components/Common" -I"D:/STM32CubeMX/Disco747-xav/Binn" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Synth-2f-Src

clean-Synth-2f-Src:
	-$(RM) ./Synth/Src/MIDI_application.cyclo ./Synth/Src/MIDI_application.d ./Synth/Src/MIDI_application.o ./Synth/Src/MIDI_application.su ./Synth/Src/additive_osc.cyclo ./Synth/Src/additive_osc.d ./Synth/Src/additive_osc.o ./Synth/Src/additive_osc.su ./Synth/Src/adsr.cyclo ./Synth/Src/adsr.d ./Synth/Src/adsr.o ./Synth/Src/adsr.su ./Synth/Src/audio_conf.cyclo ./Synth/Src/audio_conf.d ./Synth/Src/audio_conf.o ./Synth/Src/audio_conf.su ./Synth/Src/blepvco.cyclo ./Synth/Src/blepvco.d ./Synth/Src/blepvco.o ./Synth/Src/blepvco.su ./Synth/Src/chorusFD.cyclo ./Synth/Src/chorusFD.d ./Synth/Src/chorusFD.o ./Synth/Src/chorusFD.su ./Synth/Src/delay.cyclo ./Synth/Src/delay.d ./Synth/Src/delay.o ./Synth/Src/delay.su ./Synth/Src/drift_3osc.cyclo ./Synth/Src/drift_3osc.d ./Synth/Src/drift_3osc.o ./Synth/Src/drift_3osc.su ./Synth/Src/drifter.cyclo ./Synth/Src/drifter.d ./Synth/Src/drifter.o ./Synth/Src/drifter.su ./Synth/Src/math_tools.cyclo ./Synth/Src/math_tools.d ./Synth/Src/math_tools.o ./Synth/Src/math_tools.su ./Synth/Src/metronome.cyclo ./Synth/Src/metronome.d ./Synth/Src/metronome.o ./Synth/Src/metronome.su ./Synth/Src/minblep_tables.cyclo ./Synth/Src/minblep_tables.d ./Synth/Src/minblep_tables.o ./Synth/Src/minblep_tables.su ./Synth/Src/notesTables.cyclo ./Synth/Src/notesTables.d ./Synth/Src/notesTables.o ./Synth/Src/notesTables.su ./Synth/Src/osc.cyclo ./Synth/Src/osc.d ./Synth/Src/osc.o ./Synth/Src/osc.su ./Synth/Src/phaser.cyclo ./Synth/Src/phaser.d ./Synth/Src/phaser.o ./Synth/Src/phaser.su ./Synth/Src/random.cyclo ./Synth/Src/random.d ./Synth/Src/random.o ./Synth/Src/random.su ./Synth/Src/rational_ratios.cyclo ./Synth/Src/rational_ratios.d ./Synth/Src/rational_ratios.o ./Synth/Src/rational_ratios.su ./Synth/Src/resonantFilter.cyclo ./Synth/Src/resonantFilter.d ./Synth/Src/resonantFilter.o ./Synth/Src/resonantFilter.su ./Synth/Src/sequencer.cyclo ./Synth/Src/sequencer.d ./Synth/Src/sequencer.o ./Synth/Src/sequencer.su ./Synth/Src/sinetable.cyclo ./Synth/Src/sinetable.d ./Synth/Src/sinetable.o ./Synth/Src/sinetable.su ./Synth/Src/soundgen.cyclo ./Synth/Src/soundgen.d ./Synth/Src/soundgen.o ./Synth/Src/soundgen.su

.PHONY: clean-Synth-2f-Src

