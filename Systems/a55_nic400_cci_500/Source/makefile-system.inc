#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-a55 --target=aarch64-arm-none-eabi
CPU_LDFLAGS  = --entry=start64
CPU_ASFLAGS  = -mcpu=cortex-a55

############################################ GCC-A-ELF
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -march=armv8-a -mtune=cortex-a55
CPU_LDFLAGS  = -march=armv8-a -mtune=cortex-a55 --entry=start64
CPU_ASFLAGS  = -march=armv8-a -mtune=cortex-a55

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################


# Startup code to initalize processor
startup_Av8.o: $(SYSTEM_APP)/startup/startup_Av8.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

startup_Av8_aarch64.o: $(SYSTEM_APP)/startup/startup_Av8_aarch64.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

startup_Av8_vectors.o: $(SYSTEM_APP)/startup/startup_Av8_vectors.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

startup_Av8_utils.o: $(SYSTEM_APP)/startup/startup_Av8_utils.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<


# PMU code to count specific benchmark sections
pmu_Av8.o: $(SYSTEM_APP)/pmu-and-marker/pmu_Av8.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $<

OBJ_FILES += startup_Av8.o startup_Av8_aarch64.o startup_Av8_vectors.o startup_Av8_utils.o pmu_Av8.o
