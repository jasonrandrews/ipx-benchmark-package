// ------------------------------------------------------------
// PMU for ARMv8-A
// Headers
//
// Copyright (c) 2012 ARM Ltd.  All rights reserved.
// ------------------------------------------------------------

#ifndef _ARMV8_AARCH64_PMU_H
#define _ARMV8_AARCH64_PMU_H

//
// These are the events listed in section C9.10.1 of ARM DDI 0406B
// The ARM ARM provides a description for each code.  READ THESE
// BEFORE USING THEM!
//

#define V8_PMU_EVENT_SOFTWARE_INC 0x00
#define V8_PMU_EVENT_ICACHE_MISS 0x01
#define V8_PMU_EVENT_INSTR_TLB_MISS 0x02
#define V8_PMU_EVENT_DCACHE_LINE_FILL 0x03
#define V8_PMU_EVENT_DCACHE_MISS 0x04
#define V8_PMU_EVENT_DATA_TLB_MISS 0x05
#define V8_PMU_EVENT_MEM_READ 0x06
#define V8_PMU_EVENT_MEM_WRITE 0x07
#define V8_PMU_EVENT_INSTR_EXECUTED 0x08
#define V8_PMU_EVENT_EXCEPTION 0x09
#define V8_PMU_EVENT_EXCEPTION_RETURN 0x0A
#define V8_PMU_EVENT_CONTEXTID_CHANGE 0x0B
#define V8_PMU_EVENT_WRITE_TO_PC 0x0C
#define V8_PMU_EVENT_BRANCH_IMM 0x0D
#define V8_PMU_EVENT_BRANCH_DIRECT 0x0E
#define V8_PMU_EVENT_UNALIGNED_ACCESS 0x0F
#define V8_PMU_EVENT_BRANCH_MISPREDICT 0x10
#define V8_PMU_EVENT_CYCLE_COUNT 0x11
#define V8_PMU_EVENT_PREDICTABLE_BRANCH 0x12

#define V8_PMU_EVENT_FILTER_P (1 << 31)
#define V8_PMU_EVENT_FILTER_U (1 << 30)
#define V8_PMU_EVENT_FILTER_NSK (1 << 29)
#define V8_PMU_EVENT_FILTER_NSU (1 << 28)
#define V8_PMU_EVENT_FILTER_NSH (1 << 27)
#define V8_PMU_EVENT_FILTER_M (1 << 26)
#define V8_PMU_EVENT_FILTER_ALL_EL (1 << 27)

// Returns the number of progammable counters
unsigned int getPMN(void);

// Sets the event for a programmable counter to record
// counter = Which counter to program  (e.g. 0 for PMN0, 1 for PMN1)
// event   = The event code (from appropiate TRM)
// filter  = The filters to apply to this counter
void pmnConfig(unsigned int counter, unsigned int event, unsigned int filter);

// Enables/disables the divider (1/64) on CCNT
// divider = If 0 disable divider, else enable dvider
void ccntDivider(int divider);

// Sets the filters (PMCCFILTR_EL0) for the CCNT
// filter  = The filters to apply to CCNT
void setCCNTFilter(unsigned int filter);

//
// Enables and disables
//

// Global PMU enable
void enablePMU(void);

// Global PMU disable
void disablePMU(void);

// Enable the CCNT
void enableCCNT(void);

// Disable the CCNT
void disableCCNT(void);

// Enable PMNn
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void enablePMN(unsigned int counter);

// Enable PMNn
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void disablePMN(unsigned int counter);

// Disables all the programmable counters and the cycle counter
void disableAllCounters(void);

//
// Read and write counter values
//

// Returns the value of CCNT
unsigned long long readCCNT(void);

// Sets the value of CCNT
// NOTE: Should only be called when CCNT is disabled (stopped)
// count = The value to be written into CCNT
void writeCCNT(unsigned long long count);

// Returns the value of PMNn
// counter = The counter to read (e.g. 0 for PMN0, 1 for PMN1)
unsigned int readPMN(unsigned int counter);

// Sets the value of PMn
// counter = The counter to be written (e.g. 0 for PMN0, 1 for PMN1)
// count   = The value to be written
void writePMN(unsigned int counter, unsigned int count);

//
// Overflow and interrupts
//

// Returns the value of the overflow flags
unsigned int readFlags(void);

// Clears the specified overflow flags
void clearFlags(unsigned int flags);

// Enables interrupt generation on overflow of the CCNT
void enableCCNTIrq(void);

// Disables interrupt generation on overflow of the CCNT
void disableCCNTIrq(void);

// Enables interrupt generation on overflow of PMNn
// counter = The counter to enable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void enablePMNIrq(unsigned int counter);

// Disables interrupt generation on overflow of PMNn
// counter = The counter to disable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void disablePMNIrq(unsigned int counter);

//
// Counter reset functions
//

// Resets the programmable counters
void resetPMN(void);

// Resets the CCNT
void resetCCNT(void);

//
// Software Increment
//

// Writes to software increment register
// counter = The counter to increment (e.g. 0 for PMN0, 1 for PMN1)
void pmuSoftwareIncrement(unsigned int counter);

//
// User mode access
//

//
// NOTE:
// In ARMv7-A/R (PMUv1 & PMUv2) there is a single enable bit for
// User (now EL0) access.  As of PMUv3, there are four bits in this register:
//
// bit 0 - EN - User enable. Allow EL0 to read/write most PMU registers.
// bit 1 - SW - Software Increment write enable. Allow EL0 to write PMSWINC_EL0.
// bit 2 - CR - Cycle counter read enable. Allow EL0 to read PMCCNTR_EL0.
// bit 3 - ER - Event counter read enable.
//
// The original enable/disable functions are maintained, and set/clear all
// the bits.  New set/get functions are provided to give greater flexibility

// Enables User mode access to the PMU (must be called in a priviledged mode)
void enablePMUUserAccess(void);

// Disables User mode access to the PMU (must be called in a priviledged mode)
void disablePMUserAccess(void);

// Writes the PMUSERENR_EL0 register
// Config = value to write to PMUSERENR_EL0
void setPMUSERENR_EL0(unsigned int config);

// Returns value of the PMUSERENR_EL0 register
unsigned int getPMUSERENR_EL0(void);

#endif

// ------------------------------------------------------------
// End of v8_aarch64_pmu.h
// ------------------------------------------------------------
