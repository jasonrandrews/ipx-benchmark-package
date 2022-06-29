#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-r8 -mfloat-abi=hard -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --cpu=Cortex-R8 --entry=ResetHandler
CPU_ASFLAGS  = --cpu=Cortex-R8 --thumb

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mcpu=cortex-r8 -mfloat-abi=hard  -mthumb
CPU_LDFLAGS  = -mcpu=cortex-r8 --entry=ResetHandler -mfloat-abi=hard
CPU_ASFLAGS  = -mcpu=cortex-r8 -mfloat-abi=hard -mthumb

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################

# Startup code to initalize processor
$(COMPILER_DIR)/startup_R8.o: $(SYSTEM_APP)/startup/startup_R8.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# PMU code to count specific benchmark sections
$(COMPILER_DIR)/pmu_ARv7.o: $(SYSTEM_APP)/pmu-and-marker/pmu_ARv7.s
	#$(AS) $(ASFLAGS) -g $<
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_R8.o $(COMPILER_DIR)/pmu_ARv7.o
