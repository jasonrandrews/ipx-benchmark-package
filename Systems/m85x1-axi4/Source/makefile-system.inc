#*********************************************************************
#      Copyright (c) 2022-2023 by Arm Limited (or its affiliates). All Rights Reserved.
#*********************************************************************

SYSTEM_APP := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(SYSTEM_APP)/makefile/makefile-shared.inc

## Shared flags between compilers
SERIAL_DEFINES =

# The name of the CPU in the CMSIS Device directory
CMSIS_DEVICE = ARMCM85

# Specify which CPU header file we want to include used to compile CMSIS app.
DEFINES      += -D $(CMSIS_DEVICE)

############################################ AC6
ifeq ($(COMPILER),AC6)

CPU_CFLAGS   = -mthumb --target=arm-arm-none-eabi -fshort-enums -fshort-wchar
CPU_LDFLAGS  = --entry=ResetHandler

# If we don't want to compile for FPU, don't enable the flags
# Note that in M85, the FPU and MVE share registers, so we need to disable both MVE and FP instructions
# if we don't allow access to those registers
ifeq ($(NO_FPU),1)
CPU_CFLAGS  += -mcpu=cortex-m85+nomve+nofp -mfloat-abi=soft
CPU_LDFLAGS += --cpu=Cortex-M85.no_mve.no_fp
else
CPU_CFLAGS  += -mcpu=cortex-m85 -mfloat-abi=hard
CPU_LDFLAGS += --cpu=Cortex-M85
endif

# Tell the disassembler to render the .dis file with the right instruction set for the CPU
DIS_UTIL_FLAGS += --cpu=Cortex-M85

############################################ GCC-RM
else ifeq ($(COMPILER),GCC)

CPU_CFLAGS   = -mthumb -fshort-enums -fshort-wchar
# GCC Needs a specific flag to enable float formatting support
CPU_LDFLAGS  = -mthumb --entry=ResetHandler -u _printf_float

# If we don't want to compile for FPU, don't enable the flags
# The current version of the Arm GNU Toolchain (11.3rel1) does not yet suppport Cortex M85
# So we will manually enable the fp and mve options for now
ifeq ($(NO_FPU),1)
CPU_CFLAGS  += -march=armv8.1-m.main+nofp -mfloat-abi=soft
CPU_LDFLAGS += -march=armv8.1-m.main+nofp -mfloat-abi=soft
else
CPU_CFLAGS  += -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard
CPU_LDFLAGS += -march=armv8.1-m.main+mve.fp+fp.dp -mfloat-abi=hard
endif

# Tell the disassembler to render the .dis file with the right instruction set for the CPU
DIS_UTIL_FLAGS += -marmv8.1-m.main

else
$(error ERROR: COMPILER variable not valid. Must be either AC6 or GCC. Currently: $(COMPILER) )
endif
########################################################################################

# Startup code to initalize processor
$(COMPILER_DIR)/startup_M.o: $(SYSTEM_APP)/startup/startup_M.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# PMU code to count specific benchmark sections
$(COMPILER_DIR)/pmu_M.o: $(SYSTEM_APP)/pmu-and-marker/pmu_M.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) -I $(SYSTEM_APP)/compilers $< -o $@

OBJ_FILES += $(COMPILER_DIR)/startup_M.o $(COMPILER_DIR)/pmu_M.o