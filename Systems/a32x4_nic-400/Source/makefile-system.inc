#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-a32 -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --entry=Vectors
CPU_ASFLAGS  = -mcpu=cortex-a32 -mthumb

############################################ GCC-A-EABI
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -march=armv8-a -mtune=cortex-a32 -mthumb
CPU_LDFLAGS  = -march=armv8-a -mtune=cortex-a32 --entry=Vectors
CPU_ASFLAGS  = -march=armv8-a -mtune=cortex-a32 -mthumb

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################

# Startup code to initalize processor
startup_Av7.o: $(SYSTEM_APP)/startup/startup_Av7.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

# PMU code to count specific benchmark sections
pmu_ARv7.o: $(SYSTEM_APP)/pmu-and-marker/pmu_ARv7.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

OBJ_FILES += startup_Av7.o pmu_ARv7.o
