################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CLCD_program.c \
../DIO_program.c \
../MCP2515_Program.c \
../PORT_program.c \
../SPI_Program.c \
../main.c 

OBJS += \
./CLCD_program.o \
./DIO_program.o \
./MCP2515_Program.o \
./PORT_program.o \
./SPI_Program.o \
./main.o 

C_DEPS += \
./CLCD_program.d \
./DIO_program.d \
./MCP2515_Program.d \
./PORT_program.d \
./SPI_Program.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


