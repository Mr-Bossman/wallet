##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.14.1] date: [Fri Jul 30 18:28:13 EDT 2021] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = wallet


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O3


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources

 #export CC=arm-none-eabi-gcc
 #export CXX=arm-none-eabi-g++
 #./autogen.sh
 #./configure --build=arm-none-eabi --target=arm-none-eabi --host=x86_64-linux-gnu CFLAGS='--specs=nosys.specs  -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard  -Wall -fdata-sections -ffunction-sections -DUSE_ECMULT_STATIC_PRECOMPUTATION ' CXXFLAGS='--specs=nosys.specs  -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard  -Wall -fdata-sections -ffunction-sections -DUSE_ECMULT_STATIC_PRECOMPUTATION ' --enable-module-recovery
C_SOURCES =  \
Core/Src/ssd1306.c \
Core/Src/run.c \
Core/Src/retarget.c \
Core/Src/main.c \
Core/Src/encrypt.c \
Core/Src/eeprom.c \
Core/Src/sha256.c \
Core/Src/aes.c \
Core/Src/app_freertos.c \
Core/Src/stm32wbxx_it.c \
Core/Src/stm32wbxx_hal_msp.c \
Core/Src/stm32wbxx_hal_timebase_tim.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_gpio.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_hsem.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_cortex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_exti.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim_ex.c \
Core/Src/system_stm32wbxx.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_i2c.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_i2c_ex.c

# ASM sources
ASM_SOURCES =  \
startup_stm32wb55xx_cm4.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32WB55xx


# AS includes
AS_INCLUDES =  \
-ICore/Inc

# C includes
C_INCLUDES =  \
-ICore/Inc \
-ICore/Inc/font8x8 \
-IDrivers/STM32WBxx_HAL_Driver/Inc \
-IDrivers/STM32WBxx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IDrivers/CMSIS/Device/ST/STM32WBxx/Include \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -pedantic -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -pedantic -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = stm32wb55xx_flash_cm4.ld

# libraries
LIBS = -lc -lm -lnosys -lrdimon -u _printf_float
LIBDIR = 
LDFLAGS = $(MCU) -specs=nosys.specs -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
OBJECTS += secp256k1/src/libsecp256k1_la-secp256k1.o
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
