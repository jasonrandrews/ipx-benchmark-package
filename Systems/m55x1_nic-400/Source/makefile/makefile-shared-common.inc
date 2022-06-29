#########################################################################################
# Checks that needed variables are defined in the including Makefile or by the make command line
#########################################################################################
ifndef APP
    $(error The variable "APP" must be defined when building or cleaning an application)
endif
ifndef COMPILER
    $(error The variable "COMPILER" must be defined when building or cleaning an application)
endif
ifeq (, $(findstring clean,$(MAKECMDGOALS)))
  ifndef CPU
    $(error The variable "CPU" must be defined when building an applicaiton)
  endif
endif

#########################################################################################
# Set common executables and directories
#########################################################################################

# Set Linux remove command to use for cleaning
RM=rm -rf $(1)

# Paths to directories
SCRIPTS=..

#########################################################################################
# Determine system data bus width based on CPU core
#########################################################################################

# This width impacts the generation of hex files for back-door loading.
WIDTH=64
ifeq ($(CPU),$(filter $(CPU),Cortex-M0 Cortex-M0Plus Cortex-M3 Cortex-M4 Cortex-M4F Cortex-M23 Cortex-M33 Cortex-M33F Cortex-M33+AHB-Cache Cortex-M23+AHB-Cache Cortex-M0Plus+AHB-Cache Cortex-M4F+AHB-Cache))
	WIDTH=32
endif

#########################################################################################
# Determine GCC compiler based on CPU core
#########################################################################################

COMPILER_TYPE=$(COMPILER)
ifeq ($(COMPILER), GCC)

ifneq (,$(findstring Cortex-M,$(CPU)))
# Cortex-M
COMPILER_TYPE=GCC-RM

else ifneq (,$(findstring Cortex-R,$(CPU)))
ifeq ($(CPU),$(filter $(CPU),Cortex-R82))
COMPILER_TYPE=GCC-A-ELF
else
# Cortex-R
COMPILER_TYPE=GCC-RM
endif

else ifneq (,$(findstring Cortex-A,$(CPU)))
# Cortex-A
ifeq ($(CPU),$(filter $(CPU),Cortex-A5 Cortex-A7 Cortex-A9 Cortex-A15 Cortex-A32))
COMPILER_TYPE=GCC-A-EABI
else
COMPILER_TYPE=GCC-A-ELF
endif

else
# No match for Cortex-M, Cortex-R, or Cortex-A. Error
$(error ERROR: COMPILER variable for GCC not set correctly, core doesn't match expected Cortex-M, Cortex-R, or Cortex-A values. CPU is currently: $(CPU) )
endif

endif

#########################################################################################
# Determine executables and flags based on COMPILER_TYPE; check availability
#########################################################################################

############################################ AC6
ifeq ($(COMPILER_TYPE),AC6)

# executables
CC		=armclang
AS		=armasm
LD		=armlink
AR		=armar
DIS_UTIL=fromelf
BIN_UTIL=fromelf

# defines & flags
DEFINES       = $(APP_DEFINES) $(SERIAL_DEFINES) -DSERIAL
CFLAGS        = $(CPU_CFLAGS)
LDFLAGS       = $(CPU_LDFLAGS)
ASFLAGS       = $(CPU_ASFLAGS)
DIS_UTIL_FLAGS= -c $(TARGET_APP)
BIN_UTIL_FLAGS= --bin $(TARGET_APP) --output

# Directory software goes under
COMPILER_DIR=AC6

############################################ GCC-RM
else ifeq ($(COMPILER_TYPE),GCC-RM)

# executables
CC		=arm-none-eabi-gcc
AS		=arm-none-eabi-as
LD		=arm-none-eabi-gcc
AR		=arm-none-eabi-armar
DIS_UTIL=arm-none-eabi-objdump
BIN_UTIL=arm-none-eabi-objcopy

# defines & flags
DEFINES       = $(APP_DEFINES) $(SERIAL_DEFINES) -DSERIAL
CFLAGS        = $(CPU_CFLAGS) -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-exceptions -Wall
LDFLAGS       = $(CPU_LDFLAGS) -specs=nosys.specs
ASFLAGS       = $(CPU_ASFLAGS)
DIS_UTIL_FLAGS= --disassemble --source $(TARGET_APP)
BIN_UTIL_FLAGS= -O binary $(TARGET_APP)

# Directory software goes under
COMPILER_DIR=GCC

############################################ GCC-A-EABI
else ifeq ($(COMPILER_TYPE),GCC-A-EABI)

# executables
CC		=arm-none-eabi-gcc
AS		=arm-none-eabi-as
LD		=arm-none-eabi-gcc
AR		=arm-none-eabi-armar
DIS_UTIL=arm-none-eabi-objdump
BIN_UTIL=arm-none-eabi-objcopy

# defines & flags
DEFINES       = $(APP_DEFINES) $(SERIAL_DEFINES) -DSERIAL
CFLAGS        = $(CPU_CFLAGS) -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-exceptions -Wall
LDFLAGS       = $(CPU_LDFLAGS) -specs=nosys.specs
ASFLAGS       = $(CPU_ASFLAGS)
DIS_UTIL_FLAGS= --disassemble --source $(TARGET_APP)
BIN_UTIL_FLAGS= -O binary $(TARGET_APP)

# Directory software goes under
COMPILER_DIR=GCC

############################################ GCC-A-ELF
else ifeq ($(COMPILER_TYPE),GCC-A-ELF)

# executables
CC		=aarch64-none-elf-gcc
AS		=aarch64-none-elf-as
LD		=aarch64-none-elf-gcc
AR		=aarch64-none-elf-armar
DIS_UTIL=aarch64-none-elf-objdump
BIN_UTIL=aarch64-none-elf-objcopy

# defines & flags
DEFINES       = $(APP_DEFINES) $(SERIAL_DEFINES) -DSERIAL
CFLAGS        = $(CPU_CFLAGS) -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-exceptions -Wall
LDFLAGS       = $(CPU_LDFLAGS) -specs=nosys.specs
ASFLAGS       = $(CPU_ASFLAGS)
DIS_UTIL_FLAGS= --disassemble --source $(TARGET_APP)
BIN_UTIL_FLAGS= -O binary $(TARGET_APP)

# Directory software goes under
COMPILER_DIR=GCC

else

$(error ERROR: COMPILER variable not valid. Must be either AC6, GCC. Currently: $(COMPILER) with COMPILER_TYPE set to $(COMPILER_TYPE))

endif

############################################ Check Compiler Availability
# Check to make sure the selected compiler is available
# GNU compilers are not installed by default and require a script to be run to install them
ifeq (, $(shell which $(CC)))
  $(info Error: Cannot find the selected compiler, looking for $(CC))
  $(error exiting compilation)
endif

#########################################################################################
# Targets and recipes shared for all applications and build steps
#########################################################################################

TARGET_APP_NAME=$(APP).axf
TARGET_APP=$(COMPILER_DIR)/$(TARGET_APP_NAME)
