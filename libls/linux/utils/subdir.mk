################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/utils/cfghandler.cpp \
$(SRC_DIR)/utils/cmdline.cpp \
$(SRC_DIR)/utils/utils.cpp 

HEADERS += \
$(SRC_DIR)/utils/cfghandler.h \
$(SRC_DIR)/utils/cmdline.h \
$(SRC_DIR)/utils/utils.h 

OBJS += \
./utils/cfghandler.o \
./utils/cmdline.o \
./utils/utils.o 

CPP_DEPS += \
./utils/cfghandler.d \
./utils/cmdline.d \
./utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: $(SRC_DIR)/utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


