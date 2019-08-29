################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/lock/lock.cpp 

HEADERS += \
$(SRC_DIR)/core/lock/lock.h 

OBJS += \
./core/lock/lock.o 

CPP_DEPS += \
./core/lock/lock.d 


# Each subdirectory must supply rules for building sources it contributes
core/lock/%.o: $(SRC_DIR)/core/lock/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


