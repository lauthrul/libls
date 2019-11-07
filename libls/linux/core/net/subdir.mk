################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/net/httpclient.cpp 

HEADERS += \
$(SRC_DIR)/core/net/httpclient.h 

OBJS += \
./core/net/httpclient.o 

CPP_DEPS += \
./core/net/httpclient.d 


# Each subdirectory must supply rules for building sources it contributes
core/net/%.o: $(SRC_DIR)/core/net/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


