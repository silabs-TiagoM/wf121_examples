################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include WF121/subdir.mk
-include CMSIS/efm32wg/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SPI_testing.axf

# Tool invocations
SPI_testing.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "SPI_testing.ld" -Xlinker --gc-sections -Xlinker -Map="SPI_testing.map" --specs=nano.specs -o SPI_testing.axf "./src/InitDevice.o" "./src/main.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./WF121/cmd_def.o" "./WF121/stubs.o" "./CMSIS/efm32wg/startup_gcc_efm32wg.o" "./CMSIS/efm32wg/system_efm32wg.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SPI_testing.hex'
	arm-none-eabi-objcopy -O ihex "SPI_testing.axf" "SPI_testing.hex"
	@echo ' '

	@echo 'Building bin file: SPI_testing.bin'
	arm-none-eabi-objcopy -O binary "SPI_testing.axf" "SPI_testing.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SPI_testing.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) SPI_testing.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
