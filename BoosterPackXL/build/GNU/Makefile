############################################################################
# Product: Makefile for QP/C on EK-TM4C123GXL, uC/OS-II kernel, GNU-ARM
# Last Updated for Version: 6.5.0
# Date of the Last Update:  2019-04-15
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
#
# This program is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Alternatively, this program may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly supersede
# the GNU General Public License and are specifically designed for
# licensees interested in retaining the proprietary status of their code.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Contact information:
# https://www.state-machine.com
# mailto:info@state-machine.com
##############################################################################
# examples of invoking this Makefile:
# building configurations: Debug (default), Release, and Spy
# make
# make CONF=rel
# make CONF=spy
#
# cleaning configurations: Debug (default), Release, and Spy
# make clean
# make CONF=rel clean
# make CONF=spy clean
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the Qtools collection for Windows, see:
#    https://sourceforge.net/projects/qpc/files/QTools/
#

#-----------------------------------------------------------------------------
# project name
#
PROJECT     := BoosterPack

Project_PATH := ../../
ThirdParty_PATH := ../../third_party
CMSIS_PATH := $(ThirdParty_PATH)/CMSIS/Core/Include
DriverLib_PATH := $(ThirdParty_PATH)/TI/driverlib
Tiva_PATH := $(ThirdParty_PATH)/TI
GRLIB_PATH := $(ThirdParty_PATH)/TI/grlib
GRLIB_FONTS_PATH := $(GRLIB_PATH)/fonts
RTOS_PATH := $(ThirdParty_PATH)/uC-OS2
RTOS_SRC_PATH := $(RTOS_PATH)/uC-OS2
RTOS_CPU_PATH := $(RTOS_PATH)/uC-CPU
RTOS_LIB_PATH := $(RTOS_PATH)/uC-LIB

# list of all source directories used by this project
VPATH = \
	$(Project_PATH)src \
	$(Project_PATH)cfg \
	$(DriverLib_PATH) \
	$(GRLIB_PATH) \
	$(GRLIB_FONTS_PATH) \
	$(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M \
	$(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M/GNU \
	$(RTOS_SRC_PATH)/Source \
	$(RTOS_CPU_PATH) \
	$(RTOS_CPU_PATH)/ARM-Cortex-M/ARMv7-M \
	$(RTOS_CPU_PATH)/ARM-Cortex-M/ARMv7-M/GNU \
	$(RTOS_LIB_PATH) \
	$(RTOS_LIB_PATH)/Ports/ARM-Cortex-M4/GNU

# list of all include directories needed by this project
INCLUDES  = \
	-I"$(Project_PATH)inc" \
	-I"$(Project_PATH)cfg" \
	-I"$(CMSIS_PATH)" \
	-I"$(Tiva_PATH)"  \
	-I"$(GRLIB_PATH)"  \
	-I$(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M \
	-I$(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M/GNU \
	-I$(RTOS_SRC_PATH)/Source \
	-I$(RTOS_CPU_PATH) \
	-I$(RTOS_CPU_PATH)/ARM-Cortex-M/ARMv7-M/GNU \
	-I$(RTOS_LIB_PATH) \
	-I$(RTOS_LIB_PATH)/Ports/ARM-Cortex-M4/GNU

#-----------------------------------------------------------------------------
# files
#

# assembler source files
ASM_SRCS := \
	cpu_a.s \
	os_cpu_a.s #\
	#lib_mem_a.s

# C source files
C_SRCS := \
	$(wildcard $(Project_PATH)/src/*.c)  \
	$(wildcard $(Project_PATH)/cfg/*.c) 

DRIVERLIB_SRCS := \
	$(wildcard $(DriverLib_PATH)/*.c) 

GRLIB_SRCS := \
	$(wildcard $(GRLIB_PATH)/*.c) \
	$(wildcard $(GRLIB_FONTS_PATH)/*.c) 

RTOS_SRCS := \
	os_dbg.c \
	os_core.c \
	os_flag.c \
	os_mbox.c \
	os_mem.c \
	os_mutex.c \
	os_q.c \
	os_sem.c \
	os_task.c \
	os_time.c \
	os_tmr.c \
	os_cpu_c.c \
	cpu_c.c \
	cpu_core.c \
	lib_ascii.c \
	lib_math.c \
	lib_mem.c \
	lib_str.c
# RTOS_SRCS := \
# 	$(wildcard $(Project_PATH)cfg/*.c) \
# 	$(wildcard $(RTOS_SRC_PATH)/Source/*.c) \
# 	$(wildcard $(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M/*.c) \
# 	$(wildcard $(RTOS_SRC_PATH)/Ports/ARM-Cortex-M/ARMv7-M/GNU/*.c) \
# 	$(wildcard $(RTOS_CPU_PATH)/*.c) \
# 	$(wildcard $(RTOS_CPU_PATH)/ARM-Cortex-M/ARMv7-M/*.c) \
# 	$(wildcard $(RTOS_LIB_PATH)/*.c) \

# UTILS_SRCS := \
# 	uartstdio.c

OUTPUT    := $(PROJECT)
LD_SCRIPT := $(PROJECT).ld


# LIB_DIRS  := $(DriverLib_PATH)/gcc
# LIBS      := $(DriverLib_PATH)/gcc/libdriver.a

# defines
DEFINES   := -DTARGET_IS_TM4C123_RB1 -D__FPU_PRESENT -Dgcc

# ARM CPU, ARCH, FPU, and Float-ABI types...
# ARM_CPU:   [cortex-m0 | cortex-m0plus | cortex-m1 | cortex-m3 | cortex-m4]
# ARM_ARCH:  [6 | 7] (NOTE: must match ARM_CPU!)
# ARM_FPU:   [ | vfp]
# FLOAT_ABI: [ | soft | softfp | hard]
#
ARM_CPU   := -mcpu=cortex-m4
ARM_ARCH  := 7   # NOTE: must match the ARM_CPU!
ARM_FPU   := -mfpu=vfp
FLOAT_ABI := -mfloat-abi=softfp
ASM_FPU   := -defsym=__FPU_PRESENT=1

#-----------------------------------------------------------------------------
# GNU-ARM toolset (NOTE: You need to adjust to your machine)
# see https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
#
# GNU_ARM := C:/"Program Files (x86)"/"Arm GNU Toolchain arm-none-eabi"/"14.2 rel1"/
GNU_ARM := /usr/


CC    := $(GNU_ARM)/bin/arm-none-eabi-gcc
AS    := $(GNU_ARM)/bin/arm-none-eabi-as
LINK  := $(GNU_ARM)/bin/arm-none-eabi-gcc
BIN   := $(GNU_ARM)/bin/arm-none-eabi-objcopy


##############################################################################
# Typically, you should not need to change anything below this line

# basic utilities (included in Qtools for Windows), see:
#    http://sourceforge.net/projects/qpc/files/Qtools

MKDIR := mkdir
RM    := rm

#-----------------------------------------------------------------------------
# build options for various configurations for ARM Cortex-M4F
#

# combine all the soruces...
C_SRCS += $(DRIVERLIB_SRCS)
C_SRCS += $(GRLIB_SRCS)
C_SRCS += $(RTOS_SRCS)

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := rel

ASFLAGS = $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O1 $(INCLUDES) $(DEFINES) -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -DQ_SPY

else # default Debug configuration ..........................................

BIN_DIR := dbg

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -funwind-tables -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES)

endif # ......................................................................


LINKFLAGS = -T$(LD_SCRIPT) $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections $(LIB_DIRS)


ASM_OBJS     := $(patsubst %.s,%.o,  $(notdir $(ASM_SRCS)))
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_BIN)
#all: $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(BIN) -O binary $< $@

$(TARGET_ELF) : $(ASM_OBJS_EXT) $(C_OBJS_EXT)
	$(LINK) $(LINKFLAGS) -o $@ $^  $(LIBS)

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT)
  endif
endif


.PHONY : clean
clean:
# -del /Q $(BIN_DIR)\*.o \
# $(BIN_DIR)\*.d \
# $(BIN_DIR)\*.bin \
# $(BIN_DIR)\*.elf \
# $(BIN_DIR)\*.map
	rm -f $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.bin \
	$(BIN_DIR)/*.elf \
	$(BIN_DIR)/*.map

	
show:
	@echo PROJECT = $(PROJECT)
	@echo CONF = $(CONF)
	@echo DEFINES = $(DEFINES)
	@echo ASM_FPU = $(ASM_FPU)
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo C_SRCS = $(C_SRCS)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo TARGET_ELF = $(TARGET_ELF)
