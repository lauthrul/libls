################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/lock/lock.cpp 

HEADERS += \
../core/lock/lock.h 

OBJS += \
./core/lock/lock.o 

CPP_DEPS += \
./core/lock/lock.d 


# Each subdirectory must supply rules for building sources it contributes
core/lock/%.o: ../core/lock/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


