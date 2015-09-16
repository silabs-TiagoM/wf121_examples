################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BGLib/cmd_def.c \
../BGLib/stubs.c 

OBJS += \
./BGLib/cmd_def.o \
./BGLib/stubs.o 

C_DEPS += \
./BGLib/cmd_def.d \
./BGLib/stubs.d 


# Each subdirectory must supply rules for building sources it contributes
BGLib/cmd_def.o: ../BGLib/cmd_def.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb '-DEFM32WG990F256=1' -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/wonder_gecko_spi_host/inc" -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/wonder_gecko_spi_host/BGLib" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/EFM32WG_STK3800/config" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -MMD -MP -MF"BGLib/cmd_def.d" -MT"BGLib/cmd_def.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

BGLib/stubs.o: ../BGLib/stubs.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb '-DEFM32WG990F256=1' -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/wonder_gecko_spi_host/inc" -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/wonder_gecko_spi_host/BGLib" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/EFM32WG_STK3800/config" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -MMD -MP -MF"BGLib/stubs.d" -MT"BGLib/stubs.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


