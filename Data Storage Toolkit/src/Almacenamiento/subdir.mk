################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Almacenamiento/Archivo.cpp \
../src/Almacenamiento/Buffer.cpp \
../src/Almacenamiento/EABloques.cpp \
../src/Almacenamiento/EARegistros.cpp \
../src/Almacenamiento/EATexto.cpp 

OBJS += \
./src/Almacenamiento/Archivo.o \
./src/Almacenamiento/Buffer.o \
./src/Almacenamiento/EABloques.o \
./src/Almacenamiento/EARegistros.o \
./src/Almacenamiento/EATexto.o 

CPP_DEPS += \
./src/Almacenamiento/Archivo.d \
./src/Almacenamiento/Buffer.d \
./src/Almacenamiento/EABloques.d \
./src/Almacenamiento/EARegistros.d \
./src/Almacenamiento/EATexto.d 


# Each subdirectory must supply rules for building sources it contributes
src/Almacenamiento/%.o: ../src/Almacenamiento/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


