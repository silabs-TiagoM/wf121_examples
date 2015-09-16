################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/efm32wg/subdir.mk
-include BGLib/subdir.mk
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
all: wonder_gecko_spi_host.axf

# Tool invocations
wonder_gecko_spi_host.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "wonder_gecko_spi_host.ld" -Xlinker --gc-sections -Xlinker -Map="wonder_gecko_spi_host.map" --specs=nano.specs -o wonder_gecko_spi_host.axf "./src/InitDevice.o" "./src/main.o" "./src/printSWO.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_int.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./Drivers/gpiointerrupt.o" "./Drivers/retargetio.o" "./CMSIS/efm32wg/startup_gcc_efm32wg.o" "./CMSIS/efm32wg/system_efm32wg.o" "./BGLib/cmd_def.o" "./BGLib/stubs.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: wonder_gecko_spi_host.hex'
	arm-none-eabi-objcopy -O ihex "wonder_gecko_spi_host.axf" "wonder_gecko_spi_host.hex"
	@echo ' '

	@echo 'Building bin file: wonder_gecko_spi_host.bin'
	arm-none-eabi-objcopy -O binary "wonder_gecko_spi_host.axf" "wonder_gecko_spi_host.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "wonder_gecko_spi_host.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) wonder_gecko_spi_host.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
