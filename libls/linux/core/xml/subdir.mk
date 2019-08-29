################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/xml/tinystr.cpp \
$(SRC_DIR)/core/xml/tinyxml.cpp \
$(SRC_DIR)/core/xml/tinyxmlerror.cpp \
$(SRC_DIR)/core/xml/tinyxmlparser.cpp \
$(SRC_DIR)/core/xml/xml.cpp 

HEADERS += \
$(SRC_DIR)/core/xml/tinystr.h \
$(SRC_DIR)/core/xml/tinyxml.h \
$(SRC_DIR)/core/xml/xml.h 

OBJS += \
./core/xml/tinystr.o \
./core/xml/tinyxml.o \
./core/xml/tinyxmlerror.o \
./core/xml/tinyxmlparser.o \
./core/xml/xml.o 

CPP_DEPS += \
./core/xml/tinystr.d \
./core/xml/tinyxml.d \
./core/xml/tinyxmlerror.d \
./core/xml/tinyxmlparser.d \
./core/xml/xml.d 


# Each subdirectory must supply rules for building sources it contributes
core/xml/%.o: $(SRC_DIR)/core/xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


