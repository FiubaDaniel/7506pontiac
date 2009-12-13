################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/Compuesto/Bloque.o \
../src/Compuesto/BloqueDisco.o \
../src/Compuesto/BloqueMemoria.o \
../src/Compuesto/Registro.o \
../src/Compuesto/RegistroDisco.o \
../src/Compuesto/RegistroMemoria.o 

CPP_SRCS += \
../src/Compuesto/Bloque.cpp \
../src/Compuesto/BloqueDisco.cpp \
../src/Compuesto/BloqueMemoria.cpp \
../src/Compuesto/Registro.cpp \
../src/Compuesto/RegistroDisco.cpp \
../src/Compuesto/RegistroMemoria.cpp 

OBJS += \
./src/Compuesto/Bloque.o \
./src/Compuesto/BloqueDisco.o \
./src/Compuesto/BloqueMemoria.o \
./src/Compuesto/Registro.o \
./src/Compuesto/RegistroDisco.o \
./src/Compuesto/RegistroMemoria.o 

CPP_DEPS += \
./src/Compuesto/Bloque.d \
./src/Compuesto/BloqueDisco.d \
./src/Compuesto/BloqueMemoria.d \
./src/Compuesto/Registro.d \
./src/Compuesto/RegistroDisco.d \
./src/Compuesto/RegistroMemoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/Compuesto/%.o: ../src/Compuesto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


