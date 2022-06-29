#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

############################################ Common
# Settings common to all compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --entry=ResetHandler
CPU_ASFLAGS  = -mthumb

CPU_CFLAGS   += -mcpu=cortex-m33+nodsp -mfloat-abi=soft
CPU_LDFLAGS  += --cpu=Cortex-M33.no_dsp --fpu=SoftVFP
CPU_ASFLAGS  += --cpu=Cortex-M33.no_dsp --fpu=SoftVFP

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mthumb
CPU_LDFLAGS  = --entry=ResetHandler
CPU_ASFLAGS  = -mthumb

CPU_CFLAGS   += -mcpu=cortex-m33+nodsp
CPU_LDFLAGS  += -mcpu=cortex-m33+nodsp
CPU_ASFLAGS  += -mcpu=cortex-m33+nodsp

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
