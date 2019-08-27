################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/thread/idle_thread.cpp \
../core/thread/thread.cpp 

HEADERS += \
../core/thread/idle_thread.h \
../core/thread/thread.h

OBJS += \
./core/thread/idle_thread.o \
./core/thread/thread.o 

CPP_DEPS += \
./core/thread/idle_thread.d \
./core/thread/thread.d 


# Each subdirectory must supply rules for building sources it contributes
core/thread/%.o: ../core/thread/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


