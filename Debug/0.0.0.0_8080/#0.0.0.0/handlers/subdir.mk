################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../0.0.0.0_8080/#0.0.0.0/handlers/main.c 

OBJS += \
./0.0.0.0_8080/#0.0.0.0/handlers/main.o 

C_DEPS += \
./0.0.0.0_8080/#0.0.0.0/handlers/main.d 


# Each subdirectory must supply rules for building sources it contributes
0.0.0.0_8080/#0.0.0.0/handlers/%.o: ../0.0.0.0_8080/#0.0.0.0/handlers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


