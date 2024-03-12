################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Accelerometer_Component.c \
../source/LED_Component.c \
../source/Motor_Control_Component.c \
../source/RC_Receiver_Component.c \
../source/Terminal_Component.c \
../source/freertos_hello.c \
../source/fsl_fxos.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/Accelerometer_Component.d \
./source/LED_Component.d \
./source/Motor_Control_Component.d \
./source/RC_Receiver_Component.d \
./source/Terminal_Component.d \
./source/freertos_hello.d \
./source/fsl_fxos.d \
./source/semihost_hardfault.d 

OBJS += \
./source/Accelerometer_Component.o \
./source/LED_Component.o \
./source/Motor_Control_Component.o \
./source/RC_Receiver_Component.o \
./source/Terminal_Component.o \
./source/freertos_hello.o \
./source/fsl_fxos.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DFRDM_K66F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\drivers" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\device" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\utilities" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\component\uart" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\component\serial_manager" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\component\lists" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\CMSIS" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\freertos\freertos_kernel\include" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\source" -I"C:\Users\raufr\Desktop\4DS4 Group70\4DS4-Best-Project\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Accelerometer_Component.d ./source/Accelerometer_Component.o ./source/LED_Component.d ./source/LED_Component.o ./source/Motor_Control_Component.d ./source/Motor_Control_Component.o ./source/RC_Receiver_Component.d ./source/RC_Receiver_Component.o ./source/Terminal_Component.d ./source/Terminal_Component.o ./source/freertos_hello.d ./source/freertos_hello.o ./source/fsl_fxos.d ./source/fsl_fxos.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

