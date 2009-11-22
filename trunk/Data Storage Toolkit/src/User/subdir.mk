################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/User/MisDatos.cpp 

OBJS += \
./src/User/MisDatos.o 

CPP_DEPS += \
./src/User/MisDatos.d 


# Each subdirectory must supply rules for building sources it contributes
src/User/%.o: ../src/User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


