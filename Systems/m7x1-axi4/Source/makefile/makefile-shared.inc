#*********************************************************************
# Copyright (c) 2022-2023 by Arm Limited (or its affiliates). All Rights Reserved.
#*********************************************************************
#########################################################################################
# Include the common makefile for compiler setup
#########################################################################################

include $(SYSTEM_APP)/makefile/makefile-shared-common.inc
ifeq (, $(findstring clean,$(MAKECMDGOALS)))
  ifndef SYSTEM
    $(error The variable "SYSTEM" must be defined when building an applicaiton)
  endif
endif

#########################################################################################
# Determine executables and flags based on COMPILER_TYPE; check availability
#########################################################################################

############################################ AC6
ifeq ($(COMPILER_TYPE),AC6)

# system specific build files
LINKFILE     =$(SYSTEM_APP)/scatter.scat
LINKFILEFLAG = --scatter=$(LINKFILE) --map --list=$(COMPILER_DIR)/$(APP).map

############################################ GCC-RM
else ifeq ($(COMPILER_TYPE),GCC-RM)

# system specific build files
LINKFILE =$(SYSTEM_APP)/gcc.ld
LINKFILEFLAG = -T $(LINKFILE) -Wl,-Map=$(COMPILER_DIR)/$(APP).map

############################################ GCC-A-EABI
else ifeq ($(COMPILER_TYPE),GCC-A-EABI)

# system specific build files
LINKFILE =$(SYSTEM_APP)/gcc.ld
LINKFILEFLAG = -T $(LINKFILE) -Wl,-Map=$(COMPILER_DIR)/$(APP).map

############################################ GCC-A-ELF
else ifeq ($(COMPILER_TYPE),GCC-A-ELF)

# system specific build files
LINKFILE =$(SYSTEM_APP)/gcc.ld
LINKFILEFLAG = -T $(LINKFILE) -Wl,-Map=$(COMPILER_DIR)/$(APP).map

else

$(error ERROR: COMPILER variable not valid. Must be either AC6, GCC. Currently: $(COMPILER) with COMPILER_TYPE set to $(COMPILER_TYPE))

endif

#########################################################################################
# Targets and recipes shared for all applications for building
#########################################################################################

TARGET_TAR=$(SYSTEM_APP)/../$(APP)/$(COMPILER)/$(APP)_$(COMPILER).tgz
TARGET_DIS=$(COMPILER_DIR)/$(APP).dis
TARGET_PAR=$(COMPILER_DIR)/$(APP).app_params
TARGET_APP_DATA=$(COMPILER_DIR)/app_data.json

############################################ Top-level targets

.PHONY: all rebuild clean .generate_app_params .print_info $(TARGET_APP_DATA)

# https://www.gnu.org/software/make/manual/html_node/Secondary-Expansion.html#Secondary-Expansion
.SECONDEXPANSION:
all: $(TARGET_TAR)

# The application build is dependent on externally defined variables. These variables are
# also set to the APP_DEFINES variable. Since these impact the build, they must be taken into
# account when determining dependencies for the build. The .generate_app_params target
# writes a file, $(TARGET_PAR) with the params ONLY IF the params change. This lets
# us make the build's object files dependent on these parameters and only rebuild them
# IF the params change. This target, however, must be called first for every
# build in case $(TARGET_PAR) file needs to be updated. This must happen 'outside' the build
# of the package or the package will be rebuilt regardless.
$(TARGET_TAR): .generate_app_params .print_info $(TARGET_APP) $(TARGET_DIS) $(TARGET_APP_DATA)
	@if [ -f $@ ]; then echo "Warning: $@ exists and will be removed."; rm $@; fi;
	mkdir -p $(dir $@)
	tar czf $@ -C $(COMPILER_DIR) --exclude='*Source' --exclude='*.o' --exclude='*.a' .

rebuild: clean all

.print_info:
	@echo "BUILD_INFO: "
	@echo "BUILD_INFO: Building application '$(APP)' for system '$(SYSTEM)' with compiler $(COMPILER)."
	@echo "BUILD_INFO: "
	@echo "BUILD_INFO: The following build variables are used when compiling C code and may be set or appended. They are currently set to:"
	@echo "BUILD_INFO: 	OPTIMIZATIONS	  = $(OPTIMIZATIONS)"
	@echo "BUILD_INFO: 	DEFINES 	  = $(DEFINES)"
	@echo "BUILD_INFO: 	CFLAGS 		  = $(CFLAGS)"
	@echo "BUILD_INFO: "
	@echo "BUILD_INFO: The following build variables are used when linking object files and may be set or appended. They are currently set to:"
	@echo "BUILD_INFO: 	LDFLAGS 	  = $(LDFLAGS)"
	@echo "BUILD_INFO: "
	@echo "BUILD_INFO: The following build variables are set by the system makefile and may be used when building an application:"
	@echo "BUILD_INFO: 	COMPILER_TYPE: 	  The type of compiler used for the requested compiler and system. Currently set to '$(COMPILER_TYPE)'."
	@echo "BUILD_INFO: 	COMPILER_DIR: 	  The working directory for the system makefiles where object files and the executable are placed. Currently set to '$(COMPILER_DIR)'."
	@echo "BUILD_INFO: 	CC:		  The C++ compiler used for compiling C code into object files. Currently set to '$(CC)'."
	@echo "BUILD_INFO: "

CLEAN_FILES=$(COMPILER_DIR)
clean:
	$(call RM,$(CLEAN_FILES))

############################################ Defines No FPU
# In systems with FPU, it is enabled by default in hardware and software
# To disable it in software, set "NO_FPU" to 1 before including this makefile
NO_FPU?=0

# If NO_FPU is set, this will stop fpu-init.c from enabling the FPU
ifeq ($(NO_FPU),1)
DEFINES += -DNO_FPU
endif

############################################ Shared file compiling

# Adds ability to use PL011 UART
$(COMPILER_DIR)/pl011_uart.o: $(SYSTEM_APP)/pl011_uart/pl011_uart.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) $< -o $@

# Adds printing functionality & ability to exit simulations in submain
$(COMPILER_DIR)/serial.o: $(SYSTEM_APP)/serial/serial.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) -I $(SYSTEM_APP)/pl011_uart $< -o $@

# Performs processor-specific cache initalizations in submain
$(COMPILER_DIR)/cache-init.o: $(SYSTEM_APP)/cache-init/cache-init.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) -I $(SYSTEM_APP)/serial $< -o $@

# Performs processor-specific fpu initalizations in submain
$(COMPILER_DIR)/fpu-init.o: $(SYSTEM_APP)/fpu-init/fpu-init.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) -I $(SYSTEM_APP)/serial $< -o $@

# Adds ability to mark start and stop of a piece of software for performance analysis
$(COMPILER_DIR)/pmu-and-marker.o: $(SYSTEM_APP)/pmu-and-marker/pmu-and-marker.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) \
	-I $(SYSTEM_APP)/serial \
	-I $(SYSTEM_APP) \
	-I $(SYSTEM_APP)/compilers $< -o $@

# Adds an area of code between the processor specific bootcode and the main function
$(COMPILER_DIR)/submain.o: $(SYSTEM_APP)/submain/submain.c $(TARGET_PAR)
	$(CC) -c $(OPTIMIZATIONS) $(CFLAGS) $(DEFINES) \
	-I $(SYSTEM_APP)/serial \
	-I $(SYSTEM_APP)/cache-init \
	-I $(SYSTEM_APP)/fpu-init \
	-I $(SYSTEM_APP)/pl011_uart \
	-I $(SYSTEM_APP)/compilers $< -o $@

OBJ_FILES += $(COMPILER_DIR)/pl011_uart.o $(COMPILER_DIR)/serial.o $(COMPILER_DIR)/cache-init.o $(COMPILER_DIR)/fpu-init.o $(COMPILER_DIR)/pmu-and-marker.o $(COMPILER_DIR)/submain.o

############################################ System configuration app parameters dependency

$(TARGET_APP_DATA):
	@mkdir -p $(COMPILER_DIR);
	@echo { \
		\"application\": \"$(APP)\", \
		\"compiler\": \"$(COMPILER)\", \
		\"compiler_dir\": \"$(COMPILER_DIR)\", \
		\"executable\": \"$(TARGET_APP_NAME)\", \
		\"package_id\": \"$(GENERATING_PACKAGE_ID)\", \
		\"package_version\": \"$(GENERATING_PACKAGE_VERSION)\" \
	} > $@;

# Macros for updating $(TARGET_PAR) ONLY IF the contents of the file will have changed.
dep_tmp = $(COMPILER_DIR)/tmp.$(2)
dep_file = $(COMPILER_DIR)/$(1).$(2)
create_app_params_tmp = echo $(APP_DEFINES) | tr -s " \t" "\n" | sort > $(call dep_tmp,$(1),$(2))
update_app_dep = $(call create_$(2)_tmp,$(1),$(2),$(3),$(4)) ; ( [ ! -f $(call dep_file,$(1),$(2)) ] || ! diff $(call dep_tmp,$(1),$(2)) $(call dep_file,$(1),$(2)) ) && cp $(call dep_tmp,$(1),$(2)) $(call dep_file,$(1),$(2)) && cat $(call dep_file,$(1),$(2)) ; rm -f $(call dep_tmp,$(1),$(2))

# Generate the $(TARGET_PAR) file ONLY IF the contents of the file have changed. This, along
# with making the $(OBJ_FILES) depend on $(TARGET_PAR) makes the application build dependent
# on the passed in APP_DEFINES. As a result, the application is rebuilt if the parameters change.
.generate_app_params:
	@echo "Re-generating $(TARGET_PAR) if the file will have changed."
	@mkdir -p $(COMPILER_DIR);
	@$(call update_app_dep,$(APP),app_params);

# See also this line at the end of each Application Makefile.
# $(OBJ_FILES): $(TARGET_PAR)
# Would like to have it or one with $$(OBJ_FILES) here instead before the $(TARGET_APP).
# target below.

############################################ Linking

# OBJ_FILES relies on .SECONDEXPANSION used in the 'all' target above.
# LDFLAGS must be listed after OBJ_FILES in case a library is listed as part of them
$(TARGET_APP): $$(OBJ_FILES) $(LINKFILE)
	$(LD) $(LINKFILEFLAG) $(OBJ_FILES) $(LDFLAGS) -o $@

############################################ Disassembling

$(TARGET_DIS): $(TARGET_APP)
	# Create disassembly file
	$(DIS_UTIL) $(DIS_UTIL_FLAGS) > $@
