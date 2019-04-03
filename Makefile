

######################################
# target
######################################
TARGET = sonar_firmware


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

# These all have to be defined for your specific processor
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard

PROJ_DIR := ./
MCU_SERIES := f3
MCU_PARTNO := STM32F303xC

# Project specific source directories, can be empty
SOURCE_DIRS :=


# link script
LDSCRIPT = $(PROJ_DIR)/LinkerScript.ld


# Source and header files are in same directory
LIBCANARD_DIR := $(PROJ_DIR)/libcanard
LIBCANARD_STM32_DIR := $(LIBCANARD_DIR)/drivers/stm32

CANARD_DSDL_COMPILED_DIR := $(PROJ_DIR)/libcanard_dsdlc_generated

HAL_DRIVER_DIR := HAL_Driver
HAL_DRIVER_INC_DIR := $(HAL_DRIVER_DIR)/Inc
HAL_DRIVER_SRC_DIR := $(HAL_DRIVER_DIR)/Src

CMSIS_DIR := CMSIS
CMSIS_CORE_DIR := $(CMSIS_DIR)/core
CMSIS_DEVICE_DIR := $(CMSIS_DIR)/device


SOURCE_DIRS += \
		$(HAL_DRIVER_SRC_DIR) \
		$(PROJ_DIR)/src \
		$(PROJ_DIR)/adafruit_si5351 \
		$(LIBCANARD_DIR) \
		$(LIBCANARD_STM32_DIR) \


# Find C sources:
C_SOURCES := $(shell find $(SOURCE_DIRS) -maxdepth 1 ! -name "*template.c" -name "*.c")
# DSDL compiled sources need to be treated differently
C_SOURCES += $(shell find $(CANARD_DSDL_COMPILED_DIR) -name "*.c")

# Find C++ sources
CPP_SOURCES := $(shell find $(SOURCE_DIRS) -maxdepth 1 -name "*.cpp")

# Startup file, must be changed for different processors
ASM_SOURCES := $(PROJ_DIR)/startup/startup_stm32.s


##############
# Build tools
##############
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)


# Definitions to be called when compiling
AS_DEFS :=

# Common to C and C++
C_DEFS := \
	-DUSE_HAL_DRIVER \
	-D$(MCU_PARTNO) \


# Includes for compiling
AS_INCLUDES := 


# Common to C and C++
C_INCLUDES := \
	-I$(PROJ_DIR)/inc \
	-I$(HAL_DRIVER_INC_DIR) \
	-I$(CMSIS_CORE_DIR) \
	-I$(CMSIS_DEVICE_DIR) \
	-I$(PROJ_DIR)/adafruit_si5351 \
	-I$(LIBCANARD_DIR) \
	-I$(LIBCANARD_STM32_DIR) \
	-I$(CANARD_DSDL_COMPILED_DIR) \


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -std=gnu99 
CXXFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -std=c++11

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
CXXFLAGS += -g -gdwarf-2
endif


# Generate dependency information
# As far as I understand, this makes it so you can recompile correctly when header files change
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


##################
# Linker stuff!
##################

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

ifdef ARM_MATH
	LIBS += -larm_math
	LIBDIR += -L$(CMSIS_DIR)/lib
endif



#################
# Building!
#################
BUILD_DIR := $(PROJ_DIR)/build

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# C++ objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
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


####################
# Flashing/Debugging
####################

flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f $(COMMON_DIR)/$(MCU_SERIES)_openocd.cfg \
		-c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit" \

debug: $(BUILD_DIR)/$(TARGET).elf
	openocd -f $(COMMON_DIR)/$(MCU_SERIES)_openocd.cfg


#########################
# Generating DSDL
#########################

.PHONY: dsdl

dsdl:
	$(LIBCANARD_DIR)/dsdl_compiler/libcanard_dsdlc --outdir $(CANARD_DSDL_COMPILED_DIR) \
			$(PROJ_DIR)/dsdl/uavcan $(PROJ_DIR)/custom_dsdl/arvp 


#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
