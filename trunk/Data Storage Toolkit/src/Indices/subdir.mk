################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Indices/EstrategiaBSharp.cpp \
../src/Indices/HashingExt.cpp 

OBJS += \
./src/Indices/EstrategiaBSharp.o \
./src/Indices/HashingExt.o 

CPP_DEPS += \
./src/Indices/EstrategiaBSharp.d \
./src/Indices/HashingExt.d 


# Each subdirectory must supply rules for building sources it contributes
src/Indices/%.o: ../src/Indices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


