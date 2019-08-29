################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/zip/zipunzip/unzip.cpp \
$(SRC_DIR)/core/zip/zipunzip/zip.cpp 

#HEADERS += \
#$(SRC_DIR)/core/zip/zipunzip/unzip.h \
#$(SRC_DIR)/core/zip/zipunzip/zip.h 

OBJS += \
./core/zip/zipunzip/unzip.o \
./core/zip/zipunzip/zip.o 

CPP_DEPS += \
./core/zip/zipunzip/unzip.d \
./core/zip/zipunzip/zip.d 


# Each subdirectory must supply rules for building sources it contributes
core/zip/zipunzip/%.o: $(SRC_DIR)/core/zip/zipunzip/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


