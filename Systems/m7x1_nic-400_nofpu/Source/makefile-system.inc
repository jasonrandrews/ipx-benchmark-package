#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

############################################ Common
# Settings common to all compilers
SERIAL_DEFINES =

ifdef DCACHE_SIZE
  DEFINES += -DDCACHE_SIZE=$(DCACHE_SIZE)
endif

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-m7 -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --entry=ResetHandler
CPU_ASFLAGS  = -mthumb

CPU_CFLAGS   += -mfloat-abi=soft
CPU_LDFLAGS  += --cpu=Cortex-M7.no_fp
CPU_ASFLAGS  += --cpu=Cortex-M7.no_fp

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mcpu=cortex-m7 -mthumb
CPU_LDFLAGS  = -mcpu=cortex-m7 --entry=ResetHandler
CPU_ASFLAGS  = -mcpu=cortex-m7 -mthumb

CPU_CFLAGS   += -mfloat-abi=soft
CPU_LDFLAGS  += -mfloat-abi=soft
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
