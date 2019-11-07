################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/time/timer.cpp 

HEADERS += \
$(SRC_DIR)/core/time/timer.h 

OBJS += \
./core/time/timer.o 

CPP_DEPS += \
./core/time/timer.d 


# Each subdirectory must supply rules for building sources it contributes
core/time/%.o: $(SRC_DIR)/core/time/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


