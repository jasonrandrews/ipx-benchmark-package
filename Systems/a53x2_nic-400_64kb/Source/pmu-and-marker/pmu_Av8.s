//==================================================================
// Copyright ARM Ltd 2014. All rights reserved.
//
// ARMv8 example - PMU Helper Functions Code
//==================================================================

        .section  PMU,"ax"
        .align 3

// ------------------------------------------------------------

  .global getPMN
  .type   getPMN, @function
  // Returns the number of progammable counters
  // uint32_t getPMN(void)
getPMN:
  MRS     x0, PMCR_EL0
  UBFX    w0, w0, #11, #5        // Extract the N field from PMCR_EL0
  RET



  .global  pmnConfig
  .type   pmnConfig, @function
  // Sets the event for a programmable counter to record
  // void pmnConfig(uint32_t counter, uint32_t event, uint32_t filter)//
  // counter = x0 = Which counter to program  (e.g. 0 for PMN0, 1 for PMN1)
  // event   = x1 = The event code
  // filter  = x2 = The filters to apply to this counter
pmnConfig:
  MSR     PMSELR_EL0, x0         // Write counter number to selection register
  ISB                            // Synchronize context
  AND     w1, w1, #0x3FF         // Mask based on event field size
  AND     w2, w2, #0xFC000000    // Mask based on the filter bits
  ORR     w1, w1, w2             // Combine the event code and filter
  // If PMU counting at EL2 is needed, uncomment the following line
  // ORR     w1, w1, #(1 << 27)     // Set the NSH bit to enable counting in EL2
  MSR     PMXEVTYPER_EL0, x1     // Write event code
  ISB
  RET




  .global ccntDivider
  .type   ccntDivider, @function
  // Enables/disables the divider (1/64) on CCNT
  // void ccntDivider(uint32_t divider)
  // divider = x0 = ?
ccntDivider:
  // TBD



  .global  setCCNTFilter
  .type   setCCNTFilter, @function
  // Sets the event for a programmable counter to record
  // void setCCNTFilter(uint32_t filter)//
  // filter = x0 = the filters to apply to CCNT
setCCNTFilter:
  AND     w0, w0, #0xFC000000    // Mask based on the filter bits
  MSR     PMCCFILTR_EL0, x0      // Write event code
  ISB
  RET


  // ---------------------------------------------------------------
  // Enable/Disable
  // These 3 lines enable Cortex-A55 PMU if running in EL3:
  //  MRS     x0, MDCR_EL3     // Read MDCR_EL3
  //  ORR     x0, x0, #1<<17   // Set SPME
  //  MSR     MDCR_EL3, x0     // Write MDCR_EL3
  // ---------------------------------------------------------------

  .global enablePMU
  .type   enablePMU, @function
  // Global PMU enable
  // void enablePMU(void)
enablePMU:
  MRS     x0, PMCR_EL0            // Read PMCR_EL0 (Performance Monitors Control Register)
  ORR     w0, w0, #0x01           // Set E bit
  MSR     PMCR_EL0, x0            // Write PMCR_EL0
  ISB
  RET



  .global disablePMU
  .type   disablePMU, @function
  // Global PMU disable
  // void disablePMU(void)
disablePMU:
  MRS     x0, PMCR_EL0            // Read PMCR_EL0 (Performance Monitors Control Register)
  BFI     w0, wzr, #0, #1         // Clear E bit
  MSR     PMCR_EL0, x0            // Write PMCR_EL0
  ISB
  RET



  .global enableCCNT
  .type   enableCCNT, @function
  // Enable the CCNT
  // void enableCCNT(void)
enableCCNT:
  // If PMU counting at EL2 is needed, uncomment the following three lines
  // MRS     x0, PMCCFILTR_EL0
  // ORR     w0, w0, #(1 << 27)      // Set the NSH bit to enable counting in EL2
  // MSR     PMCCFILTR_EL0, x0       // Write back the filter
  MOV     w0, #(1 << 31)          // Set C bit
  MSR     PMCNTENSET_EL0, x0      // Write PMCNTENSET_EL0 (Count Enable Set register)
  ISB
  RET



  .global disableCCNT
  .type   disableCCNT, @function
  // Disable the CCNT
  // void disableCCNT(void)
disableCCNT:
  MOV     w0, #(1 << 31)          // Set C bit
  MSR     PMCNTENCLR_EL0, x0      // Write PMCNTENCLR_EL0 (Count Enable Clear register)
  ISB
  RET



  .global enablePMN
  .type   enablePMN, @function
  // Enable PMNn
  // void enablePMN(uint32_t counter)
  // counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
enablePMN:
  MOV     w1, #0x1
  LSL     w0, w1, w0              // Shift the bit into the correct position for counter

  MSR     PMCNTENSET_EL0, x0      // Write PMCNTENSET_EL0 (Count Enable Set register)
  ISB
  RET



  .global disablePMN
  .type   disablePMN, @function
  // Disable PMNn
  // void disablePMN(uint32_t counter)
  // counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
disablePMN:
  MOV     w1, #0x1
  LSL     w0, w1, w0              // Shift the bit into the correct position for counter

  MSR     PMCNTENCLR_EL0, x0      // Write PMCNTENCLR_EL0 (Count Enable Clear register)
  ISB
  RET


  .global disableAllCounters
  .type   disableAllCounters, @function
  // Disables all the programmable counters and the cycle counter
  // void disableAllCounters(void)
disableAllCounters:
  //LDR     x0, =0xFFFFFFFFFFFFFFFF
  MVN     x0, xzr
  MSR     PMCNTENCLR_EL0, x0      // Write PMCNTENCLR_EL0 (Count Enable Clear register)
  ISB
  RET


  // ---------------------------------------------------------------
  // EL0 Access
  // ---------------------------------------------------------------

  // EL0 access to the PMU registers is controlled by PMUSERENR_EL0
  // In ARMv7-A/R (PMUv1 & PMUv2) the control was a single bit (bit 0)
  // In PMUv3 there are four control bits.
  // For compatibility the basic enable/disable functions are maintained,
  // but new config functions also added.

  .global  enablePMUUserAccess
  .type   enablePMUUserAccess, @function
  // Enables User mode access to the PMU (must be called in a privileged mode)
  // void enablePMUUserAccess(void)
enablePMUUserAccess:
  MRS     x0, PMUSERENR_EL0       // Read PMUSERENR_EL0 (User Enable Register)
  ORR     w0, w0, #0xF            // Set EN (0), SW (1), CR (2) and ER (3) bits
  MSR     PMUSERENR_EL0, x0       // Write PMUSERENR_EL0
  ISB
  RET



  .global  disablePMUUserAccess
  .type   disablePMUUserAccess, @function
  // Disables User mode access to the PMU (must be called in a priviledged mode)
  // void disablePMUUserAccess(void)
disablePMUUserAccess:
  MRS     x0, PMUSERENR_EL0       // Read PMUSERENR_EL0
  BFI     w0, wzr, #0, #4         // Clear EN (0), SW (1), CR (2) and ER (3) bits
  MSR     PMUSERENR_EL0, x0       // Write PMUSERENR_EL0
  ISB
  RET

/*

  .global  setPMUSERENR_EL0
  .type   setPMUSEREMR_EL0, @function
  // Writes the PMUSERENR_EL0 register
  // void setPMUSERENR_EL0(uint32_t config)
setPMUSERENR_EL0:
  MSR     PMUSERENR_EL0, x0
  ISB
  RET


  .global  getPMUSERENR_EL0
  .type   getPMUSEREMR_EL0, @function
  // Returns value of the PMUSERENR_EL0 registers
 //  uint32_t getPMUSERENR_EL0(void)
getPMUSERENR_EL0:
  MRS     x0, PMUSERENR_EL0
  RET

*/


  // ---------------------------------------------------------------
  // Read & write counters
  // ---------------------------------------------------------------

  .global readCCNT
  .type   readCCNT, @function
  // Returns the value of CCNT
  // uint64_t readCCNT(void)
readCCNT  :
  MRS     x0, PMCCNTR_EL0        // Read PMCCNTR_EL0 (Performance Monitors Cycle Counter)
  RET


  .global writeCCNT
  .type   writeCCNT, @function
  // Sets the value of CCNT
  // NOTE: Should only be called when CCNT is disabled (stopped)
  // count = x0 = The value to be written into CCNT
  // void writeCCNT(uint64_t count)//
writeCCNT:
  MSR     PMCCNTR_EL0, x0        // Write PMCCNTR_EL0 (Performance Monitors Cycle Counter)
  ISB
  RET



  .global  readPMN
  .type    readPMN, @function
  // Returns the value of PMNn
  // uint32_t readPMN(uint32_t counter)
  // counter = x0 =  The counter to read (e.g. 0 for PMN0, 1 for PMN1)
readPMN:
  MSR     PMSELR_EL0, x0         // Write counter number to selection register
  ISB                            // Synchronize context
  MRS     x0, PMXEVCNTR_EL0      // Read PMXEVCNTR_EL0 (Selected Event Counter Register)
  RET


  .global writePMN
  .type   writePMN, @function
  // Sets the value of PMn
  // void writePMN(uint32_t counter, uint32_t count)//
  // counter = x0 = The counter to be written (e.g. 0 for PMN0, 1 for PMN1)
  // count   = x1 = The value to be written
writePMN:
  MSR     PMSELR_EL0, x0         // Write counter number to selection register
  ISB                            // Synchronize context
  MSR     PMXEVCNTR_EL0, x1      // Write PMXEVCNTR_EL0 (Selected Event Counter Register)
  ISB
  RET


  // ---------------------------------------------------------------
  // Software Increment
  // ---------------------------------------------------------------

	.global pmuSoftwareIncrement
	.type   pmuSoftwareIncrement, @function
	// Writes to software increment register
	// void pmuSoftwareIncrement(uint32_t counter)
	// counter = w0 =  The counter to increment (e.g. 0 for PMN0, 1 for PMN1)
pmuSoftwareIncrement:
  MOV     w1, #0x01
  LSL			w1, w1, w0
  MSR     PMSWINC_EL0, x1        // Write PMSWINC_EL0(Software Increment register)
  ISB
  RET


  // ---------------------------------------------------------------
  // Overflow & Interrupt Generation
  // ---------------------------------------------------------------

  .global readFlags
  .type   readFlags, @function
  // Returns the value of the overflow flags
  // uint32_t readFlags(void)
readFlags:
  MRS     x0, PMOVSCLR_EL0       // Read PMOVSCLR_EL0 (Overflow Status Flags Clear register)
  RET



  .global clearFlags
  .type   clearFlags, @function
  // Clears the specified overflow flags
  // void clearFlags(uint32_t flags)
clearFlags:
  MSR     PMOVSCLR_EL0, x0       // Write PMOVSCLR_EL0 (Overflow Status Flags Clear register)
  ISB                            // Synchronize context
  RET



  .global enableCCNTIrq
  .type   enableCCNTIrq, @function
  // Enables interrupt generation on overflow of the CCNT
  // void enableCCNTIrq(void)
enableCCNTIrq:
  MOV     w0, #(1 << 31)
  MSR     PMINTENSET_EL1, x0     // Write PMINTENSET_EL1 (Interrupt Enable Set register)
  ISB
  RET



  .global disableCCNTIrq
  .type   disableCCNTIrq, @function
  // Disables interrupt generation on overflow of the CCNT
  // void disableCCNTIrq(void)
disableCCNTIrq:
  MOV     w0, #(1 << 31)
  MSR     PMINTENCLR_EL1, x0     // Write PMINTENCLR_EL1 (Interrupt Enable Clear register)
  ISB
  RET



  .global enablePMNIrq
  .type   enablePMNIrq, @function
  // Enables interrupt generation on overflow of PMNn
  // void enablePMNIrq(uint32_t counter)
  // counter = w0 =  The counter to enable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
enablePMNIrq:
  MOV     w1, #0x1
  LSL			w1, w1, w0             // Shift the bit into the correct position for counter
  MSR     PMINTENSET_EL1, x1     // Write PMINTENSET_EL1 (Interrupt Enable Set register)
  ISB
  RET



  .global disablePMNIrq
  .type   disablePMNIrq, @function
  // Disables interrupt generation on overflow of PMNn
  // void disablePMNIrq(uint32_t counter)
  // counter = w0 =  The counter to disable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
disablePMNIrq:
  MOV     w1, #0x1
  LSL			w1, w1, w0             // Shift the bit into the correct position for counter
  MSR     PMINTENCLR_EL1, x1     // Write PMINTENCLR_EL1 (Interrupt Enable Clear register)
  ISB
  RET


  // ---------------------------------------------------------------
  // Reset Functions
  // ---------------------------------------------------------------

  .global resetPMN
  .type   resetPMN, @function
  // Resets the programmable counters
  // void resetPMN(void)
resetPMN:
  MRS     x0, PMCR_EL0            // Read PMCR_EL0 (Performance Monitors Control Register)
  ORR     w0, w0, #(1 << 1)       // Set P bit (Event Counter Reset)
  MSR     PMCR_EL0, x0            // Write PMCR_EL0
  ISB
  RET



  .global  resetCCNT
  .type    resetCCNT, @function
  // Resets the CCNT
  // void resetCCNT(void)
resetCCNT:
  MRS     x0, PMCR_EL0            // Read PMCR_EL0 (Performance Monitors Control Register)
  ORR     w0, w0, #(1 << 2)       // Set C bit (Event Counter Reset)
  MSR     PMCR_EL0, x0            // Write PMCR_EL0
  ISB
  RET

// ------------------------------------------------------------
// End of v8_aarch64_pmu_armclang.s
// ------------------------------------------------------------
