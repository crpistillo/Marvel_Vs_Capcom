################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClientsThread.cpp \
../src/InputManager.cpp \
../src/Layer.cpp \
../src/MCGame.cpp \
../src/Parallax.cpp \
../src/Player.cpp \
../src/Socket.cpp \
../src/TCPServer.cpp \
../src/Team.cpp \
../src/main.cpp 

OBJS += \
./src/ClientsThread.o \
./src/InputManager.o \
./src/Layer.o \
./src/MCGame.o \
./src/Parallax.o \
./src/Player.o \
./src/Socket.o \
./src/TCPServer.o \
./src/Team.o \
./src/main.o 

CPP_DEPS += \
./src/ClientsThread.d \
./src/InputManager.d \
./src/Layer.d \
./src/MCGame.d \
./src/Parallax.d \
./src/Player.d \
./src/Socket.d \
./src/TCPServer.d \
./src/Team.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


