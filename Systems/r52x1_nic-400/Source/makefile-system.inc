#*********************************************************************
#         Copyright (c) 2022 Arm Limited. All rights reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mcpu=cortex-r52 -mfloat-abi=hard -mthumb --target=arm-arm-none-eabi
CPU_LDFLAGS  = --cpu=Cortex-R52 --entry=ResetHandler
CPU_ASFLAGS  = --cpu=Cortex-R52 -mthumb

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

#CPU_CFLAGS   = -mcpu=cortex-r52 -mthumb -mfloat-abi=hard  -mfpu=fpv5-d16
CPU_CFLAGS   = -march=armv8-a -mtune=cortex-r52 -mthumb -mfloat-abi=hard  -mfpu=fpv5-d16
CPU_LDFLAGS  = -march=armv8-a -mtune=cortex-r52 -mfloat-abi=hard --entry=ResetHandler  -mfpu=fpv5-d16
CPU_ASFLAGS  = -march=armv8-a -mtune=cortex-r52 -mthumb -mfloat-abi=hard  -mfpu=fpv5-d16

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################

# Startup code to initalize processor
$(COMPILER_DIR)/startup_R52.o: $(SYSTEM_APP)/startup/startup_R52.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# PMU code to count specific benchmark sections
$(COMPILER_DIR)/pmu_ARv7.o: $(SYSTEM_APP)/pmu-and-marker/pmu_ARv7.s
	$(CC) -c -x assembler-with-cpp $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_R52.o $(COMPILER_DIR)/pmu_ARv7.o
