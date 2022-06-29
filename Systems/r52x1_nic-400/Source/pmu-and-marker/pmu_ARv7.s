// *******************************************************************
//        Copyright (c) 2020 Arm Limited. All rights reserved.       
//                                                                   
//                         IP Selection Sandbox                      
// ******************************************************************
// PMU functions for Cortex-A/R of an Arm-v7 Architecture



  .eabi_attribute Tag_ABI_align_preserved, 1      // PRESERVE8

  .section  PMU,"x"                               // AREA  PMU,CODE,READONLY

  // no equal: ARM

// ------------------------------------------------------------
// Performance Monitor Block
// ------------------------------------------------------------
  
  .balign 8
  .global getPMN 
  .type getPMN, "function"
  getPMN:
    // Returns the number of progammable counters
    // uint32_t getPMN(void)
    MRC     p15, 0, r0, c9, c12, 0 // Read PMCR Register
    MOV     r0,r0,LSR#11           // Shift N field down to bit 0
    AND     r0, r0, #0x1F          // Mask to leave just the 5 N bits
    BX      lr

  .balign 8
  .global  pmn_config
  .type pmn_config, "function"
  // Sets the event for a programmable counter to record
  // void pmn_config(unsigned counter, uint32_t event)
  // counter (in r0) = Which counter to program (e.g. 0 for PMN0, 1 for PMN1)
  // event   (in r1) = The event code (from appropriate TRM or ARM Architecture Reference Manual)
pmn_config:
  AND     r0, r0, #0x1F          // Mask to leave only bits 4:0
  MCR     p15, 0, r0, c9, c12, 5 // Write PMSELR Register
  ISB                            // Synchronize context
  MOV     r2, #0x8000
  ORR     r1,r1,r2,LSL #12
  MCR     p15, 0, r1, c9, c13, 1 // Write PMXEVTYPER Register
  BX      lr

  .balign 8
  .global  ccnt_divider
  .type ccnt_divider, "function"
  // Enables/disables the divider (1/64) on CCNT
  // void ccnt_divider(int divider)
  // divider (in r0) = If 0 disable divider, else enable divider
ccnt_divider:
  MRC     p15, 0, r1, c9, c12, 0  // Read PMCR

  CMP     r0, #0x0                // IF (r0 == 0)
  BICEQ   r1, r1, #0x08           // THEN: Clear the D bit (disables the divisor)
  ORRNE   r1, r1, #0x08           // ELSE: Set the D bit (enables the divisor)

  MCR     p15, 0, r1, c9, c12, 0  // Write PMCR
  BX      lr
  

  // ---------------------------------------------------------------
  // Enable/Disable
  // ---------------------------------------------------------------

  .balign 8
  .global  enable_pmu
  .type enable_pmu, "function"
  // Global PMU enable
  // void enable_pmu(void)
enable_pmu:
  MRC     p15, 0, r0, c9, c12, 0  // Read PMCR
  ORR     r0, r0, #0x01           // Set E bit
  MCR     p15, 0, r0, c9, c12, 0  // Write PMCR
  BX      lr
  

  .balign 8
  .global  disable_pmu
  .type disable_pmu, "function"
  // Global PMU disable
  // void disable_pmu(void)
disable_pmu:
  MRC     p15, 0, r0, c9, c12, 0  // Read PMCR
  BIC     r0, r0, #0x01           // Clear E bit
  MCR     p15, 0, r0, c9, c12, 0  // Write PMCR
  BX      lr
  
  .balign 8
  .global  enable_ccnt
  .type enable_ccnt, "function"
  // Enable the CCNT
  // void enable_ccnt(void)
enable_ccnt:
  MOV     r0, #0x80000000         // Set C bit
  MCR     p15, 0, r0, c9, c12, 1  // Write PMCNTENSET Register
  MRC     p15, 0, r1, c14, c15, 7
  MOV     r2, #0x8000
  ORR     r1,r1,r2,LSL #12
  MCR     p15, 0, r1, c14, c15, 7 // write PMCCFILTR
  BX      lr
  

  .balign 8
  .global  disable_ccnt
  .type disable_ccnt, "function"
  // Disable the CCNT
  // void disable_ccnt(void)
disable_ccnt:
  MOV     r0, #0x80000000         // Set C bit
  MCR     p15, 0, r0, c9, c12, 2  // Write PMCNTENCLR Register
  BX      lr
  

  .balign 8
  .global  enable_pmn
  .type enable_pmn, "function"
  // Enable PMN{n}
  // void enable_pmn(uint32_t counter)
  // counter (in r0) = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
enable_pmn:
  MOV     r1, #0x1
  MOV     r1, r1, LSL r0
  MCR     p15, 0, r1, c9, c12, 1  // Write PMCNTENSET Register
  BX      lr
  

  .balign 8
  .global  disable_pmn
  .type disable_pmn, "function"
  // Disable PMN{n}
  // void disable_pmn(uint32_t counter)
  // counter (in r0) = The counter to disable (e.g. 0 for PMN0, 1 for PMN1)
disable_pmn:
  MOV     r1, #0x1
  MOV     r1, r1, LSL r0
  MCR     p15, 0, r1, c9, c12, 2  // Write PMCNTENCLR Register
  BX      lr
  

  .balign 8
  .global  enable_pmu_user_access
  .type enable_pmu_user_access, "function"
  // Enables User mode access to the PMU (must be called in a privileged mode)
  // void enable_pmu_user_access(void)
enable_pmu_user_access:
  MRC     p15, 0, r0, c9, c14, 0  // Read PMUSERENR Register
  ORR     r0, r0, #0x01           // Set EN bit (bit 0)
  MCR     p15, 0, r0, c9, c14, 0  // Write PMUSERENR Register
  ISB                             // Synchronize context
  BX      lr
  

  .balign 8
  .global  disable_pmu_user_access
  .type disable_pmu_user_access, "function"
  // Disables User mode access to the PMU (must be called in a privileged mode)
  // void disable_pmu_user_access(void)
disable_pmu_user_access: 
  MRC     p15, 0, r0, c9, c14, 0  // Read PMUSERENR Register
  BIC     r0, r0, #0x01           // Clear EN bit (bit 0)
  MCR     p15, 0, r0, c9, c14, 0  // Write PMUSERENR Register
  ISB                             // Synchronize context
  BX      lr
  


  // ---------------------------------------------------------------
  // Counter read registers
  // ---------------------------------------------------------------

  .balign 8
  .global  read_ccnt
  .type read_ccnt, "function"
  // Returns the value of CCNT
  // uint32_t read_ccnt(void)
read_ccnt:
  MRC     p15, 0, r0, c9, c13, 0 // Read CCNT Register
  BX      lr
  
  .balign 8
  .global  read_pmn
  .type read_pmn, "function"
  // Returns the value of PMN{n}
  // uint32_t read_pmn(uint32_t counter)
  // counter (in r0) = The counter to read (e.g. 0 for PMN0, 1 for PMN1)
read_pmn:
  AND     r0, r0, #0x1F          // Mask to leave only bits 4:0
  MCR     p15, 0, r0, c9, c12, 5 // Write PMSELR Register
  ISB                            // Synchronize context
  MRC     p15, 0, r0, c9, c13, 2 // Read current PMNx Register
  BX      lr
  


  // ---------------------------------------------------------------
  // Software Increment
  // ---------------------------------------------------------------

  .balign 8
  .global  pmu_software_increment
  .type pmu_software_increment, "function"
  // Writes to software increment register
  // void pmu_software_increment(uint32_t counter)
  // counter (in r0) = The counter to increment (e.g. 0 for PMN0, 1 for PMN1)
pmu_software_increment:
  MOV     r1, #0x01
  MOV     r1, r1, LSL r0
  MCR     p15, 0, r1, c9, c12, 4 // Write PMSWINCR Register
  BX      lr
  


  // ---------------------------------------------------------------
  // Overflow & Interrupt Generation
  // ---------------------------------------------------------------


  .balign 8
  .global  read_flags
  .type read_flags, "function"
  // Returns the value of the overflow flags
  // uint32_t read_flags(void)
read_flags:
  MRC     p15, 0, r0, c9, c12, 3 // Read PMOVSR Register
  BX      lr
  
  .balign 8
  .global  write_flags
  .type write_flags, "function"
  // Writes the overflow flags
  // void write_flags(uint32_t flags)
write_flags:
  MCR     p15, 0, r0, c9, c12, 3 // Write PMOVSR Register
  ISB                            // Synchronize context
  BX      lr
  

  .balign 8
  .global  enable_ccnt_irq
  .type enable_ccnt_irq, "function"
  // Enables interrupt generation on overflow of the CCNT
  // void enable_ccnt_irq(void)
enable_ccnt_irq:
  MOV     r0, #0x80000000
  MCR     p15, 0, r0, c9, c14, 1  // Write PMINTENSET Register
  BX      lr
  

  .balign 8
  .global  disable_ccnt_irq
  .type disable_ccnt_irq, "function"
  // Disables interrupt generation on overflow of the CCNT
  // void disable_ccnt_irq(void)
disable_ccnt_irq:
  MOV     r0, #0x80000000
  MCR     p15, 0, r0, c9, c14, 2   // Write PMINTENCLR Register
  BX      lr
  

  .balign 8
  .global  enable_pmn_irq
  .type enable_pmn_irq, "function"
  // Enables interrupt generation on overflow of PMN{x}
  // void enable_pmn_irq(uint32_t counter)
  // counter (in r0) = The counter to enable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
enable_pmn_irq:
  MOV     r1, #0x1
  MOV     r0, r1, LSL r0
  MCR     p15, 0, r0, c9, c14, 1   // Write PMINTENSET Register
  BX      lr
  
  .balign 8
  .global  disable_pmn_irq
  .type disable_pmn_irq, "function"
  // Disables interrupt generation on overflow of PMN{x}
  // void disable_pmn_irq(uint32_t counter)
  // counter (in r0) = The counter to disable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
disable_pmn_irq:
  MOV     r1, #0x1
  MOV     r0, r1, LSL r0
  MCR     p15, 0, r0, c9, c14, 2  // Write PMINTENCLR Register
  BX      lr
  


  // ---------------------------------------------------------------
  // Reset Functions
  // ---------------------------------------------------------------

  .balign 8
  .global  reset_pmn
  .type reset_pmn, "function"
  // Resets all programmable counters to zero
  // void reset_pmn(void)
reset_pmn:
  MRC     p15, 0, r0, c9, c12, 0  // Read PMCR
  ORR     r0, r0, #0x2            // Set P bit (Event counter reset)
  MCR     p15, 0, r0, c9, c12, 0  // Write PMCR
  BX      lr
  
  .balign 8
  .global  reset_ccnt
  .type reset_ccnt, "function"
  // Resets the CCNT
  // void reset_ccnt(void)
reset_ccnt:
  MRC     p15, 0, r0, c9, c12, 0  // Read PMCR
  ORR     r0, r0, #0x4            // Set C bit (Clock counter reset)
  MCR     p15, 0, r0, c9, c12, 0  // Write PMCR
  BX      lr
  

  .end
