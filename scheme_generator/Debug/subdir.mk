################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../routine.cpp \
../scheme_generator.cpp \
../stdafx.cpp 

OBJS += \
./routine.o \
./scheme_generator.o \
./stdafx.o 

CPP_DEPS += \
./routine.d \
./scheme_generator.d \
./stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../libls -I../../libls/utils -I../../libls/core -I../ -I"../3rd-party/MySQL Connector C 6.1/include" -include ../stdafx.h -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


