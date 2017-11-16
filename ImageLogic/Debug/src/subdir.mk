################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ImageLogic.cpp \
../src/Storm.cpp \
../src/mtcnn.cpp \
../src/network.cpp \
../src/pBox.cpp \
../src/ZBase64.cpp

OBJS += \
./src/ImageLogic.o \
./src/Storm.o \
./src/mtcnn.o \
./src/network.o \
./src/pBox.o \
./src/ZBase64.o

CPP_DEPS += \
./src/ImageLogic.d \
./src/Storm.d \
./src/mtcnn.d \
./src/network.d \
./src/pBox.d \
./src/ZBase64.d


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11  -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


