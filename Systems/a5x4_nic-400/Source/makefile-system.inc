#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-a5 --target=arm-arm-none-eabi
CPU_LDFLAGS  = --entry=Vectors
CPU_ASFLAGS  =  -g --cpu=cortex-a5

############################################ GCC-A-EABI
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -march=armv7-a -mtune=cortex-a5
CPU_LDFLAGS  = -march=armv7-a -mtune=cortex-a5 --entry=Vectors
CPU_ASFLAGS  = -march=armv7-a -mtune=cortex-a5

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################


# Startup code to initalize processor
$(COMPILER_DIR)/startup_Av7.o: $(SYSTEM_APP)/startup/startup_Av7.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# PMU code to count specific benchmark sections
$(COMPILER_DIR)/pmu_ARv7.o: $(SYSTEM_APP)/pmu-and-marker/pmu_ARv7.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_Av7.o $(COMPILER_DIR)/pmu_ARv7.o
