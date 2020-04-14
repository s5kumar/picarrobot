################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AutomatedVehicleEngine.cpp \
../ManualVehicleEngine.cpp \
../MessageProtocol.cpp \
../SocketCommunicator.cpp \
../SteeringVehicle.cpp \
../VehicleEngine.cpp \
../cGPIOHandler.cpp \
../cUltraSonicSensor.cpp \
../main.cpp 

OBJS += \
./AutomatedVehicleEngine.o \
./ManualVehicleEngine.o \
./MessageProtocol.o \
./SocketCommunicator.o \
./SteeringVehicle.o \
./VehicleEngine.o \
./cGPIOHandler.o \
./cUltraSonicSensor.o \
./main.o 

CPP_DEPS += \
./AutomatedVehicleEngine.d \
./ManualVehicleEngine.d \
./MessageProtocol.d \
./SocketCommunicator.d \
./SteeringVehicle.d \
./VehicleEngine.d \
./cGPIOHandler.d \
./cUltraSonicSensor.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


