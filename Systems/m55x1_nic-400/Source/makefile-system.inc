#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-m55 -mthumb --target=arm-arm-none-eabi -fshort-enums -fshort-wchar
CPU_LDFLAGS  = --cpu=Cortex-M55 --entry=ResetHandler
CPU_ASFLAGS  = --cpu=Cortex-M55 -mthumb

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -march=armv8-m.main+fp -mthumb -mfloat-abi=hard -fshort-enums -fshort-wchar
CPU_LDFLAGS  = -march=armv8-m.main+fp -mthumb -mfloat-abi=hard --entry=ResetHandler
CPU_ASFLAGS  = -march=armv8-m.main+fp -mthumb

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
