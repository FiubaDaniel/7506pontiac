################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/PruebaToolkit.o \
../src/Terminos.o 

CPP_SRCS += \
../src/PruebaToolkit.cpp \
../src/Terminos.cpp 

OBJS += \
./src/PruebaToolkit.o \
./src/Terminos.o 

CPP_DEPS += \
./src/PruebaToolkit.d \
./src/Terminos.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


