################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/xml/tinystr.cpp \
$(SRC_DIR)/xml/tinyxml.cpp \
$(SRC_DIR)/xml/tinyxmlerror.cpp \
$(SRC_DIR)/xml/tinyxmlparser.cpp \

HEADERS += \
$(SRC_DIR)/xml/tinystr.h \
$(SRC_DIR)/xml/tinyxml.h \

OBJS += \
./xml/tinystr.o \
./xml/tinyxml.o \
./xml/tinyxmlerror.o \
./xml/tinyxmlparser.o \

CPP_DEPS += \
./xml/tinystr.d \
./xml/tinyxml.d \
./xml/tinyxmlerror.d \
./xml/tinyxmlparser.d \


# Each subdirectory must supply rules for building sources it contributes
xml/%.o: $(SRC_DIR)/xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


