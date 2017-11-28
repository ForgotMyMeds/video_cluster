################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/C_Model_Register.cpp \
../src/CalCNN.cpp \
../src/FileProc.cpp \
../src/MainProc.cpp \
../src/ProduceDat.cpp \
../src/StdAfx.cpp 

OBJS += \
./src/C_Model_Register.o \
./src/CalCNN.o \
./src/FileProc.o \
./src/MainProc.o \
./src/ProduceDat.o \
./src/StdAfx.o 

CPP_DEPS += \
./src/C_Model_Register.d \
./src/CalCNN.d \
./src/FileProc.d \
./src/MainProc.d \
./src/ProduceDat.d \
./src/StdAfx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -Os -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


