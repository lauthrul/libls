################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/libls.cpp \
$(SRC_DIR)/stdafx.cpp 

HEADERS += \
$(SRC_DIR)/libls.h

OBJS += \
./libls.o \
./stdafx.o 

CPP_DEPS += \
./libls.d \
./stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


