################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/os/os.cpp 

HEADERS += \
../core/os/os.h 

OBJS += \
./core/os/os.o 

CPP_DEPS += \
./core/os/os.d 


# Each subdirectory must supply rules for building sources it contributes
core/os/%.o: ../core/os/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


