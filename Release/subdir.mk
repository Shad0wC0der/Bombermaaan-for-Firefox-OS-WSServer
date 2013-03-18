################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ChatBox.cpp \
../Game.cpp \
../JsonParser.cpp \
../Map.cpp \
../Message.cpp \
../Player.cpp \
../Request.cpp \
../RequestCoordinator.cpp \
../RequestFactory.cpp \
../WSServer.cpp 

OBJS += \
./ChatBox.o \
./Game.o \
./JsonParser.o \
./Map.o \
./Message.o \
./Player.o \
./Request.o \
./RequestCoordinator.o \
./RequestFactory.o \
./WSServer.o 

CPP_DEPS += \
./ChatBox.d \
./Game.d \
./JsonParser.d \
./Map.d \
./Message.d \
./Player.d \
./Request.d \
./RequestCoordinator.d \
./RequestFactory.d \
./WSServer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


