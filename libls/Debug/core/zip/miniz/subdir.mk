################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/zip/miniz/mzip.cpp 

OBJS += \
./core/zip/miniz/mzip.o 

CPP_DEPS += \
./core/zip/miniz/mzip.d 


# Each subdirectory must supply rules for building sources it contributes
core/zip/miniz/%.o: ../core/zip/miniz/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

