################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../business/NetAgent.cpp \
../business/NetManager.cpp \
../business/TaskAgent.cpp \
../business/TaskManager.cpp 

OBJS += \
./business/NetAgent.o \
./business/NetManager.o \
./business/TaskAgent.o \
./business/TaskManager.o 

CPP_DEPS += \
./business/NetAgent.d \
./business/NetManager.d \
./business/TaskAgent.d \
./business/TaskManager.d 


# Each subdirectory must supply rules for building sources it contributes
business/%.o: ../business/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../libls -I../../libls/utils -I../../libls/core -I../ -I"../3rd-party/MySQL Connector C 6.1/include" -I"../3rd-party/paho.mqtt.c/include" -I"../3rd-party/paho.mqtt.cpp/include" -include ../stdafx.h -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


