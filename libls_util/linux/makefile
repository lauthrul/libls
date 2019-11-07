################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

GCC := gcc
GCPP := g++
RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include json/subdir.mk
-include xml/subdir.mk
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

#PREFIX is environment variable, but if it is not set, then set default value
ifeq ($(PREFIX),)
PREFIX := /usr/local
endif

TARGET := libls_util.a

-include ../makefile.defs

# functions define
define fdist
	$(eval dst := $(1))
	@echo "Target path: $(dst)"
	@for h in $(HEADERS); do \
		f=$(dst)/include/$${h/"$(SRC_DIR)"/libls_util}; \
		echo "$$h -> $$f"; \
		d=`dirname $$f`; \
		mkdir -p $$d; \
		cp -f $$h $$d; \
	done
	@echo "$(TARGET) -> $(dst)/lib/$(TARGET)"
	@cp -f $(TARGET) $(dst)/lib/
endef

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(TARGET)

# Tool invocations
$(TARGET): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	ar -rc "$(TARGET)" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

install:
	@echo 'Install targets...'
	@$(call fdist, $(PREFIX))

dist:
	@echo 'Distribute targets...'
	@mkdir -p dist/include
	@mkdir -p dist/lib
	@$(call fdist, dist)

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(CC_DEPS)$(C++_DEPS)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) $(TARGET)
	-@echo ' '

post-build:
	-@echo ' '

.PHONY: all install dist clean dependents post-build

-include ../makefile.targets
