################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/string/strtool.cpp 

HEADERS += \
$(SRC_DIR)/core/string/strtool.h 

OBJS += \
./core/string/strtool.o 

CPP_DEPS += \
./core/string/strtool.d 


# Each subdirectory must supply rules for building sources it contributes
core/string/%.o: $(SRC_DIR)/core/string/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


