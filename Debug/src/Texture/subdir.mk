################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Texture/Texture.cpp 

OBJS += \
./src/Texture/Texture.o 

CPP_DEPS += \
./src/Texture/Texture.d 


# Each subdirectory must supply rules for building sources it contributes
src/Texture/%.o: ../src/Texture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


