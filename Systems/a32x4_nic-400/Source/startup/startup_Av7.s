//----------------------------------------------------------------
// Cortex-Av7 startup
//
//      Arm Compiler 6 and GCC
//----------------------------------------------------------------


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) // AC6
    #define STACK_TOP Image$$ARM_LIB_STACKHEAP$$ZI$$Limit               // Define unified variable between compilers
    #define TTB_ZI_BASE Image$$TTB$$ZI$$Base
#elif defined (__GNUC__)                                     // GCC
    #define STACK_TOP stack_top                                         // Define unified variable between compilers
    #define TTB_ZI_BASE __TTB_start
#endif


// Standard definitions of mode bits and interrupt (I & F) flags in PSRs

#define Mode_USR 0x10
#define Mode_FIQ 0x11
#define Mode_IRQ 0x12
#define Mode_SVC 0x13
#define Mode_ABT 0x17
#define Mode_UND 0x1B
#define Mode_SYS 0x1F

#define I_Bit 0x80 // When I bit is set, IRQ is disabled
#define F_Bit 0x40 // When F bit is set, FIQ is disabled


    .section  vectors, "ax"
    .align 3
    .cfi_sections .debug_frame  // put stack frame info into .debug_frame instead of .eh_frame


//----------------------------------------------------------------
// Entry point for the Reset handler
//----------------------------------------------------------------

    .global Vectors

//----------------------------------------------------------------
// Exception Vector Table
//----------------------------------------------------------------
// Note: LDR PC instructions are used here, though branch (B) instructions
// could also be used, unless the exception handlers are >32MB away.

Vectors:
    LDR PC, Reset_Addr
    LDR PC, Undefined_Addr
    LDR PC, SVC_Addr
    LDR PC, Prefetch_Addr
    LDR PC, Abort_Addr
    LDR PC, Hypervisor_Addr
    LDR PC, IRQ_Addr
    LDR PC, FIQ_Addr


    .balign 4
Reset_Addr:
    .word Reset_Handler
Undefined_Addr:
    .word Undefined_Handler
SVC_Addr:
    .word SVC_Handler
Prefetch_Addr:
    .word Prefetch_Handler
Abort_Addr:
    .word Abort_Handler
Hypervisor_Addr:
    .word Hypervisor_Handler
IRQ_Addr:
    .word IRQ_Handler
FIQ_Addr:
    .word FIQ_Handler


//----------------------------------------------------------------
// Exception Handlers
//----------------------------------------------------------------

Undefined_Handler:
    B   Undefined_Handler
SVC_Handler:
    B   SVC_Handler
Prefetch_Handler:
    B   Prefetch_Handler
Abort_Handler:
    B   Abort_Handler
Hypervisor_Handler:
    B   Hypervisor_Handler
IRQ_Handler:
    B   IRQ_Handler
FIQ_Handler:
    B   FIQ_Handler


//----------------------------------------------------------------
// Reset Handler
//----------------------------------------------------------------
Reset_Handler:

//----------------------------------------------------------------
// Disable caches and MMU in case they were left enabled from an earlier run
// This does not need to be done from a cold reset
//----------------------------------------------------------------

    MRC     p15, 0, r0, c1, c0, 0       // Read System Control Register
    BIC     r0, r0, #(0x1 << 12)        // Clear I bit 12 to disable I Cache
    BIC     r0, r0, #(0x1 <<  2)        // Clear C bit  2 to disable D Cache
    BIC     r0, r0, #0x1                // Clear M bit  0 to disable MMU
    MCR     p15, 0, r0, c1, c0, 0       // Write System Control Register
    ISB

// The MMU is enabled later, before calling main().  Caches are enabled inside main(),
// after the MMU has been enabled and scatterloading has been performed.

//----------------------------------------------------------------
// ACTLR.SMP bit must be set before the caches and MMU are enabled,
// or any cache and TLB maintenance operations are performed, even for single-core
//----------------------------------------------------------------
        MRC     p15, 0, r0, c1, c0, 1      // Read ACTLR
        ORR     r0, r0, #(1 << 6)          // Set ACTLR.SMP bit
        MCR     p15, 0, r0, c1, c0, 1      // Write ACTLR
        ISB

//----------------------------------------------------------------
// Invalidate Data and Instruction TLBs and branch predictor
// This does not need to be done from a cold reset
//----------------------------------------------------------------

        MOV     r0,#0
        MCR     p15, 0, r0, c8, c7, 0      // I-TLB and D-TLB invalidation
        MCR     p15, 0, r0, c7, c5, 6      // BPIALL - Invalidate entire branch predictor array

//----------------------------------------------------------------
// Initialize Supervisor Mode Stack
// Note stack must be 8 byte aligned.
//----------------------------------------------------------------

        LDR     SP, =STACK_TOP

//----------------------------------------------------------------
// Set Vector Base Address Register (VBAR) to point to this application's vector table
//----------------------------------------------------------------

        LDR     r0, =Vectors
        MCR     p15, 0, r0, c12, c0, 0


//----------------------------------------------------------------
// MMU Configuration
// Set translation table base
//----------------------------------------------------------------
        LDR     r0,=TTB_ZI_BASE
        MCR     p15, 0, r0, c2, c0, 0

        LDR     r0, =0xFFFFFFFF
        MCR     p15, 0, r0, c3, c0, 0      // Write Domain Access Control Register


#if defined(__ARM_NEON) || defined(__ARM_FP)
//----------------------------------------------------------------
// Enable access to NEON/VFP by enabling access to Coprocessors 10 and 11.
// Enables Full Access i.e. in both privileged and non privileged modes
//----------------------------------------------------------------

        MRC     p15, 0, r0, c1, c0, 2      // Read Coprocessor Access Control Register (CPACR)
        ORR     r0, r0, #(0xF << 20)       // Enable access to CP 10 & 11
        MCR     p15, 0, r0, c1, c0, 2      // Write Coprocessor Access Control Register (CPACR)
        ISB

//----------------------------------------------------------------
// Switch on the VFP and NEON hardware
//----------------------------------------------------------------

        MOV     r0, #0x40000000
        VMSR    FPEXC, r0                   // Write FPEXC register, EN bit set
#endif

  //
  // SMP initialization
  // -------------------
  MRC     p15, 0, r0, c0, c0, 5     // Read CPU ID register
  ANDS    r0, r0, #0x03             // Mask off, leaving the CPU ID field
  BNE     secondaryCPUsInit


//----------------------------------------------------------------
// Enable MMU and branch to __main
// Leaving the caches disabled until after scatter loading.
//----------------------------------------------------------------

        LDR     r12,=submain

        MRC     p15, 0, r0, c1, c0, 0      // Read System Control Register
        BIC     r0, r0, #(0x1 << 12)       // Clear I bit 12 to disable I Cache
        BIC     r0, r0, #(0x1 <<  2)       // Clear C bit  2 to disable D Cache
        BIC     r0, r0, #0x2               // Clear A bit  1 to disable strict alignment fault checking
//        ORR     r0, r0, #0x1               // Set M bit 0 to enable MMU before scatter loading
//        MCR     p15, 0, r0, c1, c0, 0      // Write System Control Register
        ISB

// Now the MMU is enabled, virtual to physical address translations will occur. This will affect the next
// instruction fetch.
//
// The two instructions currently in the pipeline will have been fetched before the MMU was enabled.
// The branch to __main is safe because the Virtual Address (VA) is the same as the Physical Address (PA)
// (flat mapping) of this code that enables the MMU and performs the branch

        BX      r12                        // Branch to __main C library entry point



// ------------------------------------------------------------
// Initialization for SECONDARY CPUs
// ------------------------------------------------------------

  .global secondaryCPUsInit
  .type secondaryCPUsInit, "function"
secondaryCPUsInit:
   wfi

//----------------------------------------------------------------
// Enable caches
// This code must be run from a privileged mode
//----------------------------------------------------------------

    .section  ENABLECACHES,"ax"
    .align 3

    .global enable_caches
    .type enable_caches, "function"
    .cfi_startproc
enable_caches:

//----------------------------------------------------------------
// Enable caches
//----------------------------------------------------------------

        MRC     p15, 0, r0, c1, c0, 0      // Read System Control Register
        ORR     r0, r0, #(0x1 << 12)       // Set I bit 12 to enable I Cache
        ORR     r0, r0, #(0x1 << 2)        // Set C bit  2 to enable D Cache
        MCR     p15, 0, r0, c1, c0, 0      // Write System Control Register
        ISB

        BX      lr
    .cfi_endproc

