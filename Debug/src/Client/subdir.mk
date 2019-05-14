################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client/TCPClient.cpp 

OBJS += \
./src/Client/TCPClient.o 

CPP_DEPS += \
./src/Client/TCPClient.d 


# Each subdirectory must supply rules for building sources it contributes
src/Client/%.o: ../src/Client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


