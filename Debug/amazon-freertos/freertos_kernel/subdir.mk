################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../amazon-freertos/freertos_kernel/event_groups.c \
../amazon-freertos/freertos_kernel/list.c \
../amazon-freertos/freertos_kernel/queue.c \
../amazon-freertos/freertos_kernel/stream_buffer.c \
../amazon-freertos/freertos_kernel/tasks.c \
../amazon-freertos/freertos_kernel/timers.c 

OBJS += \
./amazon-freertos/freertos_kernel/event_groups.o \
./amazon-freertos/freertos_kernel/list.o \
./amazon-freertos/freertos_kernel/queue.o \
./amazon-freertos/freertos_kernel/stream_buffer.o \
./amazon-freertos/freertos_kernel/tasks.o \
./amazon-freertos/freertos_kernel/timers.o 

C_DEPS += \
./amazon-freertos/freertos_kernel/event_groups.d \
./amazon-freertos/freertos_kernel/list.d \
./amazon-freertos/freertos_kernel/queue.d \
./amazon-freertos/freertos_kernel/stream_buffer.d \
./amazon-freertos/freertos_kernel/tasks.d \
./amazon-freertos/freertos_kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
amazon-freertos/freertos_kernel/%.o: ../amazon-freertos/freertos_kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7 -DSDK_DEBUGCONSOLE=0 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/board" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/source" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/drivers" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/device" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/CMSIS" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/amazon-freertos/freertos_kernel/include" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/utilities" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/component/serial_manager" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/component/lists" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/component/uart" -I"/home/riccus/Documents/MCUXpresso_11.1.1_3241/workspace/rt1020_freertos_hello_scheduler/xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


