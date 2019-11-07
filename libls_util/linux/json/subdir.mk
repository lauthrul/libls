################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/json/json_reader.cpp \
$(SRC_DIR)/json/json_value.cpp \
$(SRC_DIR)/json/json_writer.cpp \

HEADERS += \
$(SRC_DIR)/json/autolink.h \
$(SRC_DIR)/json/config.h \
$(SRC_DIR)/json/features.h \
$(SRC_DIR)/json/forwards.h \
$(SRC_DIR)/json/json.h \
$(SRC_DIR)/json/json_batchallocator.h \
$(SRC_DIR)/json/reader.h \
$(SRC_DIR)/json/value.h \
$(SRC_DIR)/json/writer.h \

OBJS += \
./json/json_reader.o \
./json/json_value.o \
./json/json_writer.o \

CPP_DEPS += \
./json/json_reader.d \
./json/json_value.d \
./json/json_writer.d \


# Each subdirectory must supply rules for building sources it contributes
json/%.o: $(SRC_DIR)/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


