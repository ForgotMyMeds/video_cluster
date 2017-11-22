################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Video_Single.cpp \
../src/ZBase64.cpp \
../src/mtcnn.cpp \
../src/network.cpp \
../src/pBox.cpp 

OBJS += \
./src/Video_Single.o \
./src/ZBase64.o \
./src/mtcnn.o \
./src/network.o \
./src/pBox.o 

CPP_DEPS += \
./src/Video_Single.d \
./src/ZBase64.d \
./src/mtcnn.d \
./src/network.d \
./src/pBox.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


