################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client/Client.cpp \
../src/Client/ClientSocket.cpp \
../src/Client/NetworkManager.cpp 

OBJS += \
./src/Client/Client.o \
./src/Client/ClientSocket.o \
./src/Client/NetworkManager.o 

CPP_DEPS += \
./src/Client/Client.d \
./src/Client/ClientSocket.d \
./src/Client/NetworkManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/Client/%.o: ../src/Client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


