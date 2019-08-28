################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/crypto/crypto.cpp 

C_SRCS += \
../core/crypto/aes.c \
../core/crypto/base64.c \
../core/crypto/cipher_def.c \
../core/crypto/des.c \
../core/crypto/md5.c \
../core/crypto/sha1.c \
../core/crypto/sha256.c \
../core/crypto/url_encode.c 

HEADERS += \
../core/crypto/crypto.h

OBJS += \
./core/crypto/aes.o \
./core/crypto/base64.o \
./core/crypto/cipher_def.o \
./core/crypto/crypto.o \
./core/crypto/des.o \
./core/crypto/md5.o \
./core/crypto/sha1.o \
./core/crypto/sha256.o \
./core/crypto/url_encode.o 

CPP_DEPS += \
./core/crypto/crypto.d 

C_DEPS += \
./core/crypto/aes.d \
./core/crypto/base64.d \
./core/crypto/cipher_def.d \
./core/crypto/des.d \
./core/crypto/md5.d \
./core/crypto/sha1.d \
./core/crypto/sha256.d \
./core/crypto/url_encode.d 


# Each subdirectory must supply rules for building sources it contributes
core/crypto/%.o: ../core/crypto/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/crypto/%.o: ../core/crypto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -I../core -I../utils -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


