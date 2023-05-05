#*********************************************************************
#      Copyright (c) 2022-2023 Arm Limited (or its affiliates). All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

# FPU Notes
#
# This system uses a Cortex-M7 with an FPU configured to allow either
# single-precision (32-bit) or double-precision (64-bit) floating point vector
# instructions.
#
# Use the double-precision FP build options to utilize the FPU (with 64-bit
# instructions) when type "double" is used for real numbers in a C application
# and the FPU is enabled.   If not, the soft FP is used.   When type "float" is
# used for real numbers in a C application and the FPU is enabled, the FPU is
# utilized (with 32-bit instructions) when building with either single-precision
# or double-precision build options.  This behavior holds for both the AC6 and
# the GCC compilers.
#

############################################ Common
SERIAL_DEFINES =

# The name of the CPU in the CMSIS Device directory
CMSIS_DEVICE = ARMCM7

ifdef DCACHE_SIZE
  DEFINES += -DDCACHE_SIZE=$(DCACHE_SIZE)
endif

# We defines these flag here so that CMSIS app can include the header we want
# No FPU
# DEFINES      += -D $(CMSIS_DEVICE)

# Single Precision FP
# DEFINES      += -D $(CMSIS_DEVICE)_SP

# Double precision FP
DEFINES      += -D $(CMSIS_DEVICE)_DP

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mthumb --target=arm-arm-none-eabi -fshort-enums -fshort-wchar
CPU_LDFLAGS  = --entry=ResetHandler

# If we don't want to compile for FPU, don't enable the flags
ifeq ($(NO_FPU),1)
CPU_CFLAGS  += -mcpu=cortex-m7+nofp -mfloat-abi=soft
CPU_LDFLAGS += --cpu=Cortex-M7.no_fp
else
# Double-precision FP
CPU_CFLAGS  += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-dp-d16
CPU_LDFLAGS += --cpu=Cortex-M7.fp.dp

# Single precision FP
# CPU_CFLAGS   += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16
# CPU_LDFLAGS  += --cpu=Cortex-M7.fp.sp
endif

# Tell the disassembler to render the .dis file with the right instruction set for the CPU
DIS_UTIL_FLAGS += --cpu=Cortex-M7

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mthumb -fshort-enums -fshort-wchar
# GCC Needs a specific flag to enable float formatting support
CPU_LDFLAGS  = -mthumb --entry=ResetHandler -u _printf_float

# If we don't want to compile for FPU, don't enable the flags
ifeq ($(NO_FPU),1)
CPU_CFLAGS  += -mcpu=cortex-m7+nofp -mfloat-abi=soft
CPU_LDFLAGS += -mcpu=cortex-m7+nofp -mfloat-abi=soft
else
# Double-precision FP
CPU_CFLAGS  += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
CPU_LDFLAGS += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16

# Single-precision FP
# CPU_CFLAGS   += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16
# CPU_LDFLAGS  += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16
endif

############################################ ERROR
else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif

########################################################################################

# Startup code to initalize processor
$(COMPILER_DIR)/startup_M.o: $(SYSTEM_APP)/startup/startup_M.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# PMU code to count specific benchmark sections
$(COMPILER_DIR)/pmu_M.o: $(SYSTEM_APP)/pmu-and-marker/pmu_M.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) -I $(SYSTEM_APP)/compilers $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_M.o $(COMPILER_DIR)/pmu_M.o
