################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../db/db.cpp \
../db/db_wrapper.cpp 

OBJS += \
./db/db.o \
./db/db_wrapper.o 

CPP_DEPS += \
./db/db.d \
./db/db_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
db/%.o: ../db/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../libls -I../../libls/utils -I../../libls/core -I../ -I"../3rd-party/MySQL Connector C 6.1/include" -I"../3rd-party/paho.mqtt.c/include" -I"../3rd-party/paho.mqtt.cpp/include" -include ../stdafx.h -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


