################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Indices/EstrategiaBSharp.cpp \
../src/Indices/HashingExt.cpp \
../src/Indices/Set.cpp 

OBJS += \
./src/Indices/EstrategiaBSharp.o \
./src/Indices/HashingExt.o \
./src/Indices/Set.o 

CPP_DEPS += \
./src/Indices/EstrategiaBSharp.d \
./src/Indices/HashingExt.d \
./src/Indices/Set.d 


# Each subdirectory must supply rules for building sources it contributes
src/Indices/%.o: ../src/Indices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


