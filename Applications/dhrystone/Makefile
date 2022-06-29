#
# Copyright (c) 2022 Arm Limited. All rights reserved.
#

# Include the system makefile
include ../../Systems/$(SYSTEM)/Source/makefile-system.inc

#########################################################################################
# Update the defines, cflags, ldflags, and optimizations for the application build
#########################################################################################

## Shared defines between compilers
DEFINES += -DITERATIONS=2

## AC6
ifeq ($(COMPILER_TYPE),AC6)
OPTIMIZATIONS += -g -O2
CFLAGS		  += -Wno-extern-initializer

## GCC-RM
else ifeq ($(COMPILER_TYPE),GCC-RM)
OPTIMIZATIONS += -g -O2
CFLAGS		  += -Wno-extern-initializer

## GCC-A-EABI
else ifeq ($(COMPILER_TYPE),GCC-A-EABI)
OPTIMIZATIONS += -g -O2
CFLAGS		  += -Wno-extern-initializer

## GCC-A-ELF
else ifeq ($(COMPILER_TYPE),GCC-A-ELF)
OPTIMIZATIONS += -g -O2
CFLAGS		  += -Wno-extern-initializer

else
$(error ERROR: COMPILER variable not valid. Must be either AC6, GCC. Currently: $(COMPILER) with COMPILER_TYPE set to $(COMPILER_TYPE))
endif

#########################################################################################
# Rules to build the application specific source files
#########################################################################################

$(COMPILER_DIR)/dhry_1.o: dhry_1.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

$(COMPILER_DIR)/dhry_2.o: dhry_2.c
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

OBJ_FILES += $(COMPILER_DIR)/dhry_1.o $(COMPILER_DIR)/dhry_2.o

#########################################################################################
# Required for dependencies to work correctly when parameters change
#########################################################################################

# Make each object file dependent on the system configuration application parameters.
$(OBJ_FILES): $(TARGET_PAR)
