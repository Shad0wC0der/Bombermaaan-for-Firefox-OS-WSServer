################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Bombe.cpp \
../ChatBox.cpp \
../Game.cpp \
../GameManager.cpp \
../GamePerformer.cpp \
../Item.cpp \
../ItemBombe.cpp \
../ItemPower.cpp \
../Map.cpp \
../Message.cpp \
../Player.cpp \
../RequestCoordinator.cpp \
../RequestFactory.cpp \
../RequestMessage.cpp \
../Room.cpp \
../WSRequestManager.cpp \
../WSSendManager.cpp \
../WSServer.cpp 

OBJS += \
./Bombe.o \
./ChatBox.o \
./Game.o \
./GameManager.o \
./GamePerformer.o \
./Item.o \
./ItemBombe.o \
./ItemPower.o \
./Map.o \
./Message.o \
./Player.o \
./RequestCoordinator.o \
./RequestFactory.o \
./RequestMessage.o \
./Room.o \
./WSRequestManager.o \
./WSSendManager.o \
./WSServer.o 

CPP_DEPS += \
./Bombe.d \
./ChatBox.d \
./Game.d \
./GameManager.d \
./GamePerformer.d \
./Item.d \
./ItemBombe.d \
./ItemPower.d \
./Map.d \
./Message.d \
./Player.d \
./RequestCoordinator.d \
./RequestFactory.d \
./RequestMessage.d \
./Room.d \
./WSRequestManager.d \
./WSSendManager.d \
./WSServer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.6.1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


