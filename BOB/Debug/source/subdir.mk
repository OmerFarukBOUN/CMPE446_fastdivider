################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/omerfaruk/Projects/okul/BOB/div.cpp 

OBJS += \
./source/div.o 

CPP_DEPS += \
./source/div.d 


# Each subdirectory must supply rules for building sources it contributes
source/div.o: /home/omerfaruk/Projects/okul/BOB/div.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DAESL_TB -D__llvm__ -D__llvm__ -I/home/omerfaruk/tools/Xilinx/Vitis_HLS/2023.2/include/etc -I/home/omerfaruk/Projects/okul/BOB -I/home/omerfaruk/tools/Xilinx/Vitis_HLS/2023.2/include -I/home/omerfaruk/tools/Xilinx/Vitis_HLS/2023.2/lnx64/tools/auto_cc/include -I/home/omerfaruk/tools/Xilinx/Vitis_HLS/2023.2/include/ap_sysc -I/home/omerfaruk/tools/Xilinx/Vitis_HLS/2023.2/lnx64/tools/systemc/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


