################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/thread/idle_thread.cpp \
$(SRC_DIR)/core/thread/thread.cpp 

HEADERS += \
$(SRC_DIR)/core/thread/idle_thread.h \
$(SRC_DIR)/core/thread/thread.h

OBJS += \
./core/thread/idle_thread.o \
./core/thread/thread.o 

CPP_DEPS += \
./core/thread/idle_thread.d \
./core/thread/thread.d 


# Each subdirectory must supply rules for building sources it contributes
core/thread/%.o: $(SRC_DIR)/core/thread/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


