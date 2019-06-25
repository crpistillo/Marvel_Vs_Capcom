################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Barra.cpp \
../src/Box.cpp \
../src/ClientCursor.cpp \
../src/ClientsThread.cpp \
../src/EventHandler.cpp \
../src/InputManager.cpp \
../src/Layer.cpp \
../src/MCGame.cpp \
../src/Music.cpp \
../src/Parallax.cpp \
../src/Player.cpp \
../src/RoundBanner.cpp \
../src/ServerCursor.cpp \
../src/Socket.cpp \
../src/TCPServer.cpp \
../src/Team.cpp \
../src/TimeBanner.cpp \
../src/Timer.cpp \
../src/main.cpp \
../src/signal_handler.cpp 

OBJS += \
./src/Barra.o \
./src/Box.o \
./src/ClientCursor.o \
./src/ClientsThread.o \
./src/EventHandler.o \
./src/InputManager.o \
./src/Layer.o \
./src/MCGame.o \
./src/Music.o \
./src/Parallax.o \
./src/Player.o \
./src/RoundBanner.o \
./src/ServerCursor.o \
./src/Socket.o \
./src/TCPServer.o \
./src/Team.o \
./src/TimeBanner.o \
./src/Timer.o \
./src/main.o \
./src/signal_handler.o 

CPP_DEPS += \
./src/Barra.d \
./src/Box.d \
./src/ClientCursor.d \
./src/ClientsThread.d \
./src/EventHandler.d \
./src/InputManager.d \
./src/Layer.d \
./src/MCGame.d \
./src/Music.d \
./src/Parallax.d \
./src/Player.d \
./src/RoundBanner.d \
./src/ServerCursor.d \
./src/Socket.d \
./src/TCPServer.d \
./src/Team.d \
./src/TimeBanner.d \
./src/Timer.d \
./src/main.d \
./src/signal_handler.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


