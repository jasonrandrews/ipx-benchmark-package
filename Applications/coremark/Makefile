#
# Copyright (c) 2022 Arm Limited. All rights reserved.
#

# Include the system makefile
include ../../Systems/$(SYSTEM)/Source/makefile-system.inc

#########################################################################################
# Update the defines, cflags, ldflags, and optimizations for the application build
#########################################################################################

## Shared defines between compilers
DEFINES += -DITERATIONS=2 -DMAIN_HAS_NOARGC -DCLOCK_RATE -DCYCLE_SHIFT=6 -DNDEBUG -fomit-frame-pointer -fno-common '-DFLAGS_STR="-fomit-frame-pointer -fno-common -Omax"'

## AC6
ifeq ($(COMPILER_TYPE),AC6)
OPTIMIZATIONS += -O1
DEFINES       += '-DCOMPILER_VERSION="ARM Compiler 6"'

## GCC-RM
else ifeq ($(COMPILER_TYPE),GCC-RM)
OPTIMIZATIONS += -O2
DEFINES       += '-DCOMPILER_VERSION="GCC-RM Compiler"'
LDFLAGS		  += -O2

## GCC-A-EABI
else ifeq ($(COMPILER_TYPE),GCC-A-EABI)
OPTIMIZATIONS += -O2
DEFINES       += '-DCOMPILER_VERSION="GCC-A EABI Compiler"'
LDFLAGS		  += -O2

## GCC-A-ELF
else ifeq ($(COMPILER_TYPE),GCC-A-ELF)
OPTIMIZATIONS += -Ofast
DEFINES       += '-DCOMPILER_VERSION="GCC-A ELF Compiler"'
LDFLAGS		  += -Ofast

else
$(error ERROR: COMPILER variable not valid. Must be either AC6, GCC. Currently: $(COMPILER) with COMPILER_TYPE set to $(COMPILER_TYPE))
endif

#########################################################################################
# Rules to build the application specific source files
#########################################################################################

$(COMPILER_DIR)/core_main.o: core_main.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/core_state.o: core_state.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/core_portme.o: core_portme.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/core_list_join.o: core_list_join.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/core_util.o: core_util.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/core_matrix.o: core_matrix.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES +=  $(COMPILER_DIR)/core_main.o $(COMPILER_DIR)/core_state.o $(COMPILER_DIR)/core_portme.o $(COMPILER_DIR)/core_list_join.o $(COMPILER_DIR)/core_util.o $(COMPILER_DIR)/core_matrix.o

#########################################################################################
# Required for dependencies to work correctly when parameters change
#########################################################################################

# Make each object file dependent on the system configuration application parameters.
$(OBJ_FILES): $(TARGET_PAR)
