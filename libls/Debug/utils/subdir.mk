################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../utils/cfghandler.cpp \
../utils/cmdline.cpp \
../utils/utils.cpp 

OBJS += \
./utils/cfghandler.o \
./utils/cmdline.o \
./utils/utils.o 

CPP_DEPS += \
./utils/cfghandler.d \
./utils/cmdline.d \
./utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


