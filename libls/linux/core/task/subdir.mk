################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/task/TaskAgent.cpp \
$(SRC_DIR)/core/task/TaskManager.cpp 

HEADERS += \
$(SRC_DIR)/core/task/TaskDefine.h \
$(SRC_DIR)/core/task/TaskAgent.h \
$(SRC_DIR)/core/task/TaskManager.h 

OBJS += \
./core/task/TaskAgent.o \
./core/task/TaskManager.o 

CPP_DEPS += \
./core/task/TaskAgent.d \
./core/task/TaskManager.d 


# Each subdirectory must supply rules for building sources it contributes
core/task/%.o: $(SRC_DIR)/core/task/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


