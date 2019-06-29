################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utils/subdir.mk
-include core/zip/zipunzip/subdir.mk
-include core/zip/miniz/subdir.mk
-include core/zip/subdir.mk
-include core/xml/subdir.mk
-include core/time/subdir.mk
-include core/thread/subdir.mk
-include core/string/subdir.mk
-include core/os/subdir.mk
-include core/net/subdir.mk
-include core/math/subdir.mk
-include core/logger/subdir.mk
-include core/lock/subdir.mk
-include core/crypto/subdir.mk
-include core/base64/subdir.mk
-include core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libls.dll

# Tool invocations
libls.dll: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -shared -o "libls.dll" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(CC_DEPS)$(C++_DEPS)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) libls.dll
	-@echo ' '

post-build:
	-cp  -f libls.dll ../../test/Debug/
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
