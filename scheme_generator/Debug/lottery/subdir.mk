################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lottery/lottery.cpp 

OBJS += \
./lottery/lottery.o 

CPP_DEPS += \
./lottery/lottery.d 


# Each subdirectory must supply rules for building sources it contributes
lottery/%.o: ../lottery/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../libls -I../../libls/utils -I../../libls/core -I../ -I"../3rd-party/MySQL Connector C 6.1/include" -include ../stdafx.h -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


