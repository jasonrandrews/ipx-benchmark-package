#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =


############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-r82 -target=aarch64-arm-none-eabi
CPU_LDFLAGS  = --entry=bootcode --startup=bootcode
CPU_ASFLAGS  = -mcpu=cortex-r82

############################################ GCC-A-ELF
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mcpu=cortex-r82
CPU_LDFLAGS  = -mcpu=cortex-r82  --entry=bootcode
CPU_ASFLAGS  = -mcpu=cortex-r82

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
