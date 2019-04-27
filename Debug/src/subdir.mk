################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InputManager.cpp \
../src/Layer.cpp \
../src/MCGame.cpp \
../src/Parallax.cpp \
../src/Player.cpp \
../src/Server.cpp \
../src/cliente.cpp \
../src/main.cpp 

OBJS += \
./src/InputManager.o \
./src/Layer.o \
./src/MCGame.o \
./src/Parallax.o \
./src/Player.o \
./src/Server.o \
./src/cliente.o \
./src/main.o 

CPP_DEPS += \
./src/InputManager.d \
./src/Layer.d \
./src/MCGame.d \
./src/Parallax.d \
./src/Player.d \
./src/Server.d \
./src/cliente.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


