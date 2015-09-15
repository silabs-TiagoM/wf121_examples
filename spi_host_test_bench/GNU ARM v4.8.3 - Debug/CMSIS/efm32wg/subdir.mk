################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/efm32wg/system_efm32wg.c 

S_SRCS += \
../CMSIS/efm32wg/startup_gcc_efm32wg.s 

OBJS += \
./CMSIS/efm32wg/startup_gcc_efm32wg.o \
./CMSIS/efm32wg/system_efm32wg.o 

C_DEPS += \
./CMSIS/efm32wg/system_efm32wg.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/efm32wg/%.o: ../CMSIS/efm32wg/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -c -x assembler-with-cpp -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/spi_host_test_bench/inc" -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/spi_host_test_bench/BGLib" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/EFM32WG_STK3800/config" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32WG/Include" '-DEFM32WG990F256=1' -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/efm32wg/system_efm32wg.o: ../CMSIS/efm32wg/system_efm32wg.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb '-DEFM32WG990F256=1' -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/spi_host_test_bench/inc" -I"C:/Users/timonte/Dropbox (Silicon Labs)/github/wf121/spi_host_test_bench/BGLib" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/EFM32WG_STK3800/config" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/kits/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -MMD -MP -MF"CMSIS/efm32wg/system_efm32wg.d" -MT"CMSIS/efm32wg/system_efm32wg.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


