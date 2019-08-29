################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/process/process.cpp 

HEADERS += \
$(SRC_DIR)/core/process/process.h 

OBJS += \
./core/process/process.o 

CPP_DEPS += \
./core/process/process.d 


# Each subdirectory must supply rules for building sources it contributes
core/process/%.o: $(SRC_DIR)/core/process/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


