################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/RecursoAlmacenamiento/AlmacenamientoBufferCache.o \
../src/RecursoAlmacenamiento/BufferCache.o \
../src/RecursoAlmacenamiento/EREscrituraDirecta.o \
../src/RecursoAlmacenamiento/ERUnAlmacenamiento.o \
../src/RecursoAlmacenamiento/FabricaDeRecursosAlmacenamiento.o \
../src/RecursoAlmacenamiento/Recurso.o 

CPP_SRCS += \
../src/RecursoAlmacenamiento/AlmacenamientoBufferCache.cpp \
../src/RecursoAlmacenamiento/BufferCache.cpp \
../src/RecursoAlmacenamiento/EREscrituraDirecta.cpp \
../src/RecursoAlmacenamiento/ERUnAlmacenamiento.cpp \
../src/RecursoAlmacenamiento/FabricaDeRecursosAlmacenamiento.cpp \
../src/RecursoAlmacenamiento/Recurso.cpp 

OBJS += \
./src/RecursoAlmacenamiento/AlmacenamientoBufferCache.o \
./src/RecursoAlmacenamiento/BufferCache.o \
./src/RecursoAlmacenamiento/EREscrituraDirecta.o \
./src/RecursoAlmacenamiento/ERUnAlmacenamiento.o \
./src/RecursoAlmacenamiento/FabricaDeRecursosAlmacenamiento.o \
./src/RecursoAlmacenamiento/Recurso.o 

CPP_DEPS += \
./src/RecursoAlmacenamiento/AlmacenamientoBufferCache.d \
./src/RecursoAlmacenamiento/BufferCache.d \
./src/RecursoAlmacenamiento/EREscrituraDirecta.d \
./src/RecursoAlmacenamiento/ERUnAlmacenamiento.d \
./src/RecursoAlmacenamiento/FabricaDeRecursosAlmacenamiento.d \
./src/RecursoAlmacenamiento/Recurso.d 


# Each subdirectory must supply rules for building sources it contributes
src/RecursoAlmacenamiento/%.o: ../src/RecursoAlmacenamiento/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


