################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/mmr.cpp 

HEADERS += \
$(SRC_DIR)/core/type.h \
$(SRC_DIR)/core/mmr.h 

OBJS += \
./core/mmr.o 

CPP_DEPS += \
./core/mmr.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: $(SRC_DIR)/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


