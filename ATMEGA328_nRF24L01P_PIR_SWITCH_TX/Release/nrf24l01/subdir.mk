################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nrf24l01/HardwarePlatform.cpp \
../nrf24l01/RF24.cpp \
../nrf24l01/atmega328.cpp 

OBJS += \
./nrf24l01/HardwarePlatform.o \
./nrf24l01/RF24.o \
./nrf24l01/atmega328.o 

CPP_DEPS += \
./nrf24l01/HardwarePlatform.d \
./nrf24l01/RF24.d \
./nrf24l01/atmega328.d 


# Each subdirectory must supply rules for building sources it contributes
nrf24l01/%.o: ../nrf24l01/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\nrf24l01" -I"G:\Development\workspace\ATMEGA328_nRF24L01P_PIR_SWITCH_TX\atmega328" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


