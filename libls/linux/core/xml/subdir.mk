################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/xml/tinystr.cpp \
../core/xml/tinyxml.cpp \
../core/xml/tinyxmlerror.cpp \
../core/xml/tinyxmlparser.cpp \
../core/xml/xml.cpp 

HEADERS += \
../core/xml/tinystr.h \
../core/xml/tinyxml.h \
../core/xml/xml.h 

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
core/xml/%.o: ../core/xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


