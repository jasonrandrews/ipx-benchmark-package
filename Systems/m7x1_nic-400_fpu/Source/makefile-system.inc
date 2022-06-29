#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
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

ifdef DCACHE_SIZE
  DEFINES += -DDCACHE_SIZE=$(DCACHE_SIZE)
endif

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-m7 -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --entry=ResetHandler
CPU_ASFLAGS  = -mthumb

# Double-precision FP
# CPU_CFLAGS   += -mfloat-abi=hard -mfpu=fpv5-dp-d16
# CPU_LDFLAGS  += --cpu=Cortex-M7.fp.dp
# CPU_ASFLAGS  += --cpu=Cortex-M7.fp.dp

# Single precision FP
CPU_CFLAGS   += -mfloat-abi=hard -mfpu=fpv5-sp-d16
CPU_LDFLAGS  += --cpu=Cortex-M7.fp.sp
CPU_ASFLAGS  += --cpu=Cortex-M7.fp.sp

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mcpu=cortex-m7 -mthumb
CPU_LDFLAGS  = -mcpu=cortex-m7 --entry=ResetHandler
CPU_ASFLAGS  = -mcpu=cortex-m7 -mthumb

# Double-precision FP
# CPU_CFLAGS   += -mfloat-abi=hard -mfpu=fpv5-d16
# CPU_LDFLAGS  += -mfloat-abi=hard -mfpu=fpv5-d16
# CPU_ASFLAGS  +=

# Single-precision FP
CPU_CFLAGS   += -mfloat-abi=hard -mfpu=fpv5-sp-d16
CPU_LDFLAGS  += -mfloat-abi=hard -mfpu=fpv5-sp-d16
CPU_ASFLAGS  +=

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
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_M.o $(COMPILER_DIR)/pmu_M.o
