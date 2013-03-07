################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include libraries/tkcabinet/subdir.mk
-include include/subdir.mk
-include 0.0.0.0_8080/#0.0.0.0/handlers/subdir.mk
-include 0.0.0.0_8080/#0.0.0.0/csp/folder/subdir.mk
-include 0.0.0.0_8080/#0.0.0.0/csp/subdir.mk
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
all: WuakiPlayblackReporter

# Tool invocations
WuakiPlayblackReporter: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "WuakiPlayblackReporter" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) WuakiPlayblackReporter
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
