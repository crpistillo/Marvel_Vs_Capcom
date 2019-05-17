################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Characters/Character.cpp \
../src/Characters/Spiderman.cpp \
../src/Characters/Wolverine.cpp 

OBJS += \
./src/Characters/Character.o \
./src/Characters/Spiderman.o \
./src/Characters/Wolverine.o 

CPP_DEPS += \
./src/Characters/Character.d \
./src/Characters/Spiderman.d \
./src/Characters/Wolverine.d 


# Each subdirectory must supply rules for building sources it contributes
src/Characters/%.o: ../src/Characters/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


