################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/json/json_x.cpp 

HEADERS += \
$(SRC_DIR)/core/json/json_x.h \

OBJS += \
./core/json/json_x.o 

CPP_DEPS += \
./core/json/json_x.d 


# Each subdirectory must supply rules for building sources it contributes
core/json/%.o: $(SRC_DIR)/core/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


