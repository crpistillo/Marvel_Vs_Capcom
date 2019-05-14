################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tools/FPSManager/FPSManager.cpp 

OBJS += \
./src/tools/FPSManager/FPSManager.o 

CPP_DEPS += \
./src/tools/FPSManager/FPSManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/tools/FPSManager/%.o: ../src/tools/FPSManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


