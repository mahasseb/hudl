################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HUDLDataTypes.cpp \
../src/MomentOfSnapDetector.cpp \
../src/main.cpp 

OBJS += \
./src/HUDLDataTypes.o \
./src/MomentOfSnapDetector.o \
./src/main.o 

CPP_DEPS += \
./src/HUDLDataTypes.d \
./src/MomentOfSnapDetector.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include -I/usr/include/gtk-2.0 -I/usr/lib/i386-linux-gnu/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/local/include/opencv -I/usr/local/include/gsl -I"/home/behrooz/workspace/edu.osu.hudl.mosdetector/headers" -I/home/behrooz/workspace/edu.osu.vision.common/headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


