################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tools/json/ConfigFileParser/ConfigFileParser.cpp 

OBJS += \
./src/tools/json/ConfigFileParser/ConfigFileParser.o 

CPP_DEPS += \
./src/tools/json/ConfigFileParser/ConfigFileParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/tools/json/ConfigFileParser/%.o: ../src/tools/json/ConfigFileParser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


