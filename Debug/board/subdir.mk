################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DFRDM_K66F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\drivers" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\device" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\utilities" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\component\uart" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\component\serial_manager" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\component\lists" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\CMSIS" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\freertos\freertos_kernel\include" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\source" -I"C:\Users\gironbel\Desktop\4DS4-Best-Project\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

