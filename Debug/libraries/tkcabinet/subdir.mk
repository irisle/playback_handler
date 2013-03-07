################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/tkcabinet/tcfdb.c 

OBJS += \
./libraries/tkcabinet/tcfdb.o 

C_DEPS += \
./libraries/tkcabinet/tcfdb.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/tkcabinet/%.o: ../libraries/tkcabinet/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


