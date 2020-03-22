################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GUIConf.c \
../Src/GUI_X.c \
../Src/LCDConf.c \
../Src/main.c \
../Src/rfd.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/system_stm32f7xx.c 

OBJS += \
./Src/GUIConf.o \
./Src/GUI_X.o \
./Src/LCDConf.o \
./Src/main.o \
./Src/rfd.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/system_stm32f7xx.o 

C_DEPS += \
./Src/GUIConf.d \
./Src/GUI_X.d \
./Src/LCDConf.d \
./Src/main.d \
./Src/rfd.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F746xx -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/CMSIS/Include" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/Drivers/Hardware/Inc" -I"C:/Users/Nikolina/Desktop/LIGHT_RANGER_3_STM32FF7/STemWinLib/inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


