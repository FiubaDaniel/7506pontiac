################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/Compresor/Compresor.o \
../src/Compresor/EstrategiaCompresion.o \
../src/Compresor/TablaOrden1.o \
../src/Compresor/TablaPPMC.o \
../src/Compresor/bitFile.o \
../src/Compresor/pruebas.o 

CPP_SRCS += \
../src/Compresor/Compresor.cpp \
../src/Compresor/EstrategiaCompresion.cpp \
../src/Compresor/TablaOrden1.cpp \
../src/Compresor/TablaPPMC.cpp \
../src/Compresor/bitFile.cpp \
../src/Compresor/pruebas.cpp 

OBJS += \
./src/Compresor/Compresor.o \
./src/Compresor/EstrategiaCompresion.o \
./src/Compresor/TablaOrden1.o \
./src/Compresor/TablaPPMC.o \
./src/Compresor/bitFile.o \
./src/Compresor/pruebas.o 

CPP_DEPS += \
./src/Compresor/Compresor.d \
./src/Compresor/EstrategiaCompresion.d \
./src/Compresor/TablaOrden1.d \
./src/Compresor/TablaPPMC.d \
./src/Compresor/bitFile.d \
./src/Compresor/pruebas.d 


# Each subdirectory must supply rules for building sources it contributes
src/Compresor/%.o: ../src/Compresor/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


