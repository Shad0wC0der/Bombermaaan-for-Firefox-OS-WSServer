################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Bombe.cpp \
../ChatBox.cpp \
../ClientDataRefresher.cpp \
../Game.cpp \
../GameManager.cpp \
../GamePerformer.cpp \
../Item.cpp \
../ItemBombe.cpp \
../ItemPower.cpp \
../JsonParser.cpp \
../Map.cpp \
../Message.cpp \
../Player.cpp \
../Request.cpp \
../RequestCoordinator.cpp \
../RequestFactory.cpp \
../Room.cpp \
../WSServer.cpp 

OBJS += \
./Bombe.o \
./ChatBox.o \
./ClientDataRefresher.o \
./Game.o \
./GameManager.o \
./GamePerformer.o \
./Item.o \
./ItemBombe.o \
./ItemPower.o \
./JsonParser.o \
./Map.o \
./Message.o \
./Player.o \
./Request.o \
./RequestCoordinator.o \
./RequestFactory.o \
./Room.o \
./WSServer.o 

CPP_DEPS += \
./Bombe.d \
./ChatBox.d \
./ClientDataRefresher.d \
./Game.d \
./GameManager.d \
./GamePerformer.d \
./Item.d \
./ItemBombe.d \
./ItemPower.d \
./JsonParser.d \
./Map.d \
./Message.d \
./Player.d \
./Request.d \
./RequestCoordinator.d \
./RequestFactory.d \
./Room.d \
./WSServer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.7 -I/usr/include -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


