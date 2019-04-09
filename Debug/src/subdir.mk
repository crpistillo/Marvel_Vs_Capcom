################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InputManager.cpp \
../src/Layer.cpp \
../src/MCGame.cpp \
../src/Player.cpp \
../src/main.cpp 

OBJS += \
./src/InputManager.o \
./src/Layer.o \
./src/MCGame.o \
./src/Player.o \
./src/main.o 

CPP_DEPS += \
./src/InputManager.d \
./src/Layer.d \
./src/MCGame.d \
./src/Player.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


