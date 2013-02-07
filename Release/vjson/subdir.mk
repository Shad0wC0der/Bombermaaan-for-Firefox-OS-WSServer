################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../vjson/block_allocator.cpp \
../vjson/json.cpp \
../vjson/main.cpp 

OBJS += \
./vjson/block_allocator.o \
./vjson/json.o \
./vjson/main.o 

CPP_DEPS += \
./vjson/block_allocator.d \
./vjson/json.d \
./vjson/main.d 


# Each subdirectory must supply rules for building sources it contributes
vjson/%.o: ../vjson/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.7 -I/usr/include -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


