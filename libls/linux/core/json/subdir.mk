################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/json/json_reader.cpp \
../core/json/json_value.cpp \
../core/json/json_writer.cpp \
../core/json/json_x.cpp 

HEADERS += \
../core/json/autolink.h \
../core/json/config.h \
../core/json/features.h \
../core/json/forwards.h \
../core/json/json.h \
../core/json/json_x.h \
../core/json/reader.h \
../core/json/value.h \
../core/json/writer.h

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
core/json/%.o: ../core/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


