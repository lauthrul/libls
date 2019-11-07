################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
$(SRC_DIR)/core/crypto/crypto.cpp 

C_SRCS += \
$(SRC_DIR)/core/crypto/aes.c \
$(SRC_DIR)/core/crypto/base64.c \
$(SRC_DIR)/core/crypto/cipher_def.c \
$(SRC_DIR)/core/crypto/des.c \
$(SRC_DIR)/core/crypto/md5.c \
$(SRC_DIR)/core/crypto/sha1.c \
$(SRC_DIR)/core/crypto/sha224.c \
$(SRC_DIR)/core/crypto/sha256.c \
$(SRC_DIR)/core/crypto/sha384.c \
$(SRC_DIR)/core/crypto/sha512.c \
$(SRC_DIR)/core/crypto/url_encode.c 

HEADERS += \
$(SRC_DIR)/core/crypto/crypto.h

OBJS += \
./core/crypto/aes.o \
./core/crypto/base64.o \
./core/crypto/cipher_def.o \
./core/crypto/crypto.o \
./core/crypto/des.o \
./core/crypto/md5.o \
./core/crypto/sha1.o \
./core/crypto/sha224.o \
./core/crypto/sha256.o \
./core/crypto/sha384.o \
./core/crypto/sha512.o \
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
./core/crypto/sha224.d \
./core/crypto/sha256.d \
./core/crypto/sha384.d \
./core/crypto/sha512.d \
./core/crypto/url_encode.d 


# Each subdirectory must supply rules for building sources it contributes
core/crypto/%.o: $(SRC_DIR)/core/crypto/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/crypto/%.o: $(SRC_DIR)/core/crypto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCPP) $(INCLUDE_PATH) $(DEFINES) -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


