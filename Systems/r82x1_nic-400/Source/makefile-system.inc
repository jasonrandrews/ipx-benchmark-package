#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-a55 -march=armv8.4a --target=aarch64-arm-none-eabi
CPU_LDFLAGS  = --entry=bootcode --startup=bootcode
CPU_ASFLAGS  = -mcpu=cortex-a55

############################################ GCC-A-ELF
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -march=armv8.3-a -mtune=cortex-a53
CPU_LDFLAGS  = -march=armv8.3-a -mtune=cortex-a53 --entry=bootcode
CPU_ASFLAGS  = -march=armv8.3-a -mtune=cortex-a53

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################


# Startup code to initalize processor
$(COMPILER_DIR)/startup_R82.o: $(SYSTEM_APP)/startup/startup_R82.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) -I $(SYSTEM_APP)/startup/startup_R82_includes $(DEFINES) $< -o $@

$(COMPILER_DIR)/startup_R82_vectors.o: $(SYSTEM_APP)/startup/startup_R82_vectors.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) -I $(SYSTEM_APP)/startup/startup_R82_includes $(DEFINES) $< -o $@

$(COMPILER_DIR)/startup_R82_stackheap.o: $(SYSTEM_APP)/startup/startup_R82_stackheap.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) -I $(SYSTEM_APP)/startup/startup_R82_includes $(DEFINES) $< -o $@

$(COMPILER_DIR)/pmu_Av8.o: $(SYSTEM_APP)/pmu-and-marker/pmu_Av8.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_R82.o $(COMPILER_DIR)/startup_R82_vectors.o $(COMPILER_DIR)/startup_R82_stackheap.o $(COMPILER_DIR)/pmu_Av8.o
