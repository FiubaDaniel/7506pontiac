################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tests/Compuesto/AtributoTest.cpp \
../src/tests/Compuesto/EAAlmacenamientoTest.cpp \
../src/tests/Compuesto/RegistroTest.cpp 

OBJS += \
./src/tests/Compuesto/AtributoTest.o \
./src/tests/Compuesto/EAAlmacenamientoTest.o \
./src/tests/Compuesto/RegistroTest.o 

CPP_DEPS += \
./src/tests/Compuesto/AtributoTest.d \
./src/tests/Compuesto/EAAlmacenamientoTest.d \
./src/tests/Compuesto/RegistroTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/tests/Compuesto/%.o: ../src/tests/Compuesto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


