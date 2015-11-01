################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../atmega328/usart.c 

CPP_SRCS += \
../atmega328/mtimer.cpp 

C_DEPS += \
./atmega328/usart.d 

OBJS += \
./atmega328/mtimer.o \
./atmega328/usart.o 

CPP_DEPS += \
./atmega328/mtimer.d 


# Each subdirectory must supply rules for building sources it contributes
atmega328/%.o: ../atmega328/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\nrf24l01" -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\atmega328" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

atmega328/%.o: ../atmega328/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\nrf24l01" -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\atmega328" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


