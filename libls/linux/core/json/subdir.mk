################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/json/json_reader.cpp \
$(SRC_DIR)/core/json/json_value.cpp \
$(SRC_DIR)/core/json/json_writer.cpp \
$(SRC_DIR)/core/json/json_x.cpp 

HEADERS += \
$(SRC_DIR)/core/json/autolink.h \
$(SRC_DIR)/core/json/config.h \
$(SRC_DIR)/core/json/features.h \
$(SRC_DIR)/core/json/forwards.h \
$(SRC_DIR)/core/json/json.h \
$(SRC_DIR)/core/json/json_x.h \
$(SRC_DIR)/core/json/reader.h \
$(SRC_DIR)/core/json/value.h \
$(SRC_DIR)/core/json/writer.h

OBJS += \
./core/json/json_reader.o \
./core/json/json_value.o \
./core/json/json_writer.o \
./core/json/json_x.o 

CPP_DEPS += \
./core/json/json_reader.d \
./core/json/json_value.d \
./core/json/json_writer.d \
./core/json/json_x.d 


# Each subdirectory must supply rules for building sources it contributes
core/json/%.o: $(SRC_DIR)/core/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


