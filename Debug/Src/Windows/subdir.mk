################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Windows/LightRangDLG.c 

OBJS += \
./Src/Windows/LightRangDLG.o 

C_DEPS += \
./Src/Windows/LightRangDLG.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Windows/%.o: ../Src/Windows/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F746xx -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/CMSIS/Include" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/Hardware/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/STemWinLib/inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


