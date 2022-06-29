//=============================================================================
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from Arm Limited or its affiliates.
//
//             (C) COPYRIGHT 2014 - 2021 Arm Limited or its affiliates.
//                 ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from Arm Limited or its affiliates.
//
//     Release Information : Cortex-R82 Processor MP130-r0p0-00lac0
//
//=============================================================================

        .section boot, "ax", %progbits
        .global bootcode
        .global core_synchronisation
        .global core_sync
        .global exec_cpu_var
        .global neon_fp_check
        .global neon_fp_message
        .global neon_fp_end_test
        .global end_wfi
        .global is_fuji

       .balign 16

        .type init_regs,        %function
        .type init_neon_regs,   %function
        .type initialize_sp,    %function
        .type init_imp_reg_itcm %function
        .type init_misc         %function
        .type init_mmu,         %function
        .type EL2_MPU_PROG,     %function
        .type EL1_MPU_PROG,     %function

#include "kinfu_execution_defs.hs"
#include "kinfu_mpu_macros.hs"

//-------------------------------------------------------------------------------
// Generic boot code
//-------------------------------------------------------------------------------
bootcode:
        bl      init_regs         // clear all registers
        bl      init_neon_regs    // clear all neon registers
        bl      initialize_sp     // initialize the stack pointer
        bl      init_imp_reg_itcm // initialize implemetation registers
        bl      init_misc         // Initializing miscellaneous registers
        b       init_mmu          // turn on translation

init_regs:
        mov     x0,  xzr
        mov     x1,  xzr
        mov     x2,  xzr
        mov     x3,  xzr
        mov     x4,  xzr
        mov     x5,  xzr
        mov     x6,  xzr
        mov     x7,  xzr
        mov     x8,  xzr
        mov     x9,  xzr
        mov     x10, xzr
        mov     x11, xzr
        mov     x12, xzr
        mov     x13, xzr
        mov     x14, xzr
        mov     x15, xzr
        mov     x16, xzr
        mov     x17, xzr
        mov     x18, xzr
        mov     x19, xzr
        mov     x20, xzr
        mov     x21, xzr
        mov     x22, xzr
        mov     x23, xzr
        mov     x24, xzr
        mov     x25, xzr
        mov     x26, xzr
        mov     x27, xzr
        mov     x28, xzr
        mov     x29, xzr

        // Zero the stack pointers, link registers and status registers
        mov     sp,       x0
        msr     sp_el0,   x0
        msr     sp_el1,   x0
        msr     elr_el1,  x0
        msr     elr_el2,  x0
        msr     spsr_el1, x0
        msr     spsr_el2, x0

        // Enable interrupts
        msr     DAIFClr, #0xF

        // Return
        ret

init_neon_regs:
        mrs     x0, ID_AA64PFR0_EL1
        sbfx    x5, x0, #16, #4           // Extract the floating-point field
        cmp     x5, #-1                   // x0 == -1 if FP not present
        b.eq    end_fp                    // Skip FP initialization if not present

        mrs     x1, cptr_el2
        bic     x1, x1, #(0x1 << 10)      // Ensure that CPTR_EL2.TFP is clear
        msr     cptr_el2, x1
        isb     sy

        fmov    d0,  xzr
        fmov    d1,  xzr
        fmov    d2,  xzr
        fmov    d3,  xzr
        fmov    d4,  xzr
        fmov    d5,  xzr
        fmov    d6,  xzr
        fmov    d7,  xzr
        fmov    d8,  xzr
        fmov    d9,  xzr
        fmov    d10, xzr
        fmov    d11, xzr
        fmov    d12, xzr
        fmov    d13, xzr
        fmov    d14, xzr
        fmov    d15, xzr
        fmov    d16, xzr
        fmov    d17, xzr
        fmov    d18, xzr
        fmov    d19, xzr
        fmov    d20, xzr
        fmov    d21, xzr
        fmov    d22, xzr
        fmov    d23, xzr
        fmov    d24, xzr
        fmov    d25, xzr
        fmov    d26, xzr
        fmov    d27, xzr
        fmov    d28, xzr
        fmov    d29, xzr
        fmov    d30, xzr
        fmov    d31, xzr

end_fp:
          ret

initialize_sp:
        // Initialise vector base address register for EL2
        ldr     x1, =vector_table
        msr     vbar_el2, x1
        msr     vbar_el1, x1
        // Initialize the stack pointer
        ldr     x1, =stack_top
        add     x1, x1, #4
        mrs     x2, mpidr_el1
        ubfx    x2, x2, #8, #8
        mov     x3, #CPU_STACK_SIZE
        mul     x3, x2, x3
        sub     x1, x1, x3
        mov     sp, x1
        // Also initialize EL1 stack pointer for running in EL1
        msr     sp_el1, x1
        ret

init_imp_reg_itcm:
       // ITCM pin
        mrs     x0, s3_0_c15_c0_1       // Read IMP_ITCMREGIONR_EL1
        and     x4, x0, #0x2
        cmp     x4, #0                  // Check if ITCM is enabled
        b.eq    init_imp_reg_llpp       // Don't enable DTCM region, skip to other registers
        orr     x0, x0, #0x3
        msr     s3_0_c15_c0_1, x0       // Enable ITCM at EL10 and EL2 in IMP_ITCMREGIONR_EL1

        //For DTCM
        // mov     x0, #0x3                // Enable DTCM at EL10 and EL2
        // The two lines below are a fix to the one above.  They preserve the contents of
        // fields in s3_0_c15_c0_2 that are not being modified.
        mrs     x0, s3_0_c15_c0_2       // Read IMP_DTCMREGIONR_EL1
                orr     x0, x0, #0x3    // Enable DTCM at EL10 and EL2
        msr     s3_0_c15_c0_2, x0       // Write IMP_DTCMREGIONR_EL1

init_imp_reg_llpp:
       // LLPP
        mrs     x0, s3_0_c15_c0_3       // Read IMP_LLPPREGIONR_EL1
        ubfx    x4, x0, #2, #6
        cmp     x4, #0
        b.ne    enable_LLPP
        b       init_imp_reg_spp

enable_LLPP:
        orr     x0, x0, #0x3            //Enable LLPP at EL10 and EL2
        msr     s3_0_c15_c0_3, x0

init_imp_reg_spp:
        //For  SPP
        mrs     x0, s3_0_c15_c0_5       // Read IMP_SPPREGIONR_EL1
        ubfx    x4, x0, #2, #6
        cmp     x4, #0
        b.ne    enable_SPP
        b       end_imp_reg

enable_SPP:
        orr     x0, x0, #0x3            // Enable SPP at EL10 and EL2
        msr     s3_0_c15_c0_5, x0

end_imp_reg:
        isb
        ret

init_misc:
        // Permit PMU counting at EL2
        mrs     x0, mdcr_el2            // Read MDCR_EL2
        orr     x0, x0, #1<<7           // Set HPME
        msr     mdcr_el2, x0            // Write MDCR_EL2

        // Programming Hypervisor Control Register
        mrs     x0, hcr_el2
        bic     x0, x0, #(0x1 << 27)    // TGE   (Trap General Exceptions, from EL0)         cleared
        bic     x0, x0, #(0x1 << 20)    // TACR  (Trap Auxiliary Control Register)           cleared
        bic     x0, x0, #(0x1 << 21)    // TIDCP (Trap IMPLEMENTATION DEFINED functionality) cleared
        orr     x0, x0, #(0x1 << 40)    // APK bit set to disable trapping of registers holding "key"
                                        // values for Pointer Authentication
        orr     x0, x0, #(0x1 << 41)

        mrs     x1, S3_4_C0_C0_4        // MPUIR_EL2 - If no EL2 MPU regions are found, switch to EL1 to run the test
        cmp     x1, #0
        b.eq    fiq_at_el1

        // FIQ routing at EL1 and EL2
        // Set HCR_EL2.FMO to route FIQ to EL2
        orr     x0, x0, #(0x1 << 3)
        b       write_hcr_el2

fiq_at_el1:
        // Clear HCR_EL2.FMO to NOT route FIQ to EL2
        bic     x0, x0, #(0x1 << 3)

write_hcr_el2:
        msr     hcr_el2, x0

        // Executing CPU value in MPIDR format, stored for future use
        ldr    x1, =TUBE_ADDRESS        // Executing CPU value from Trickbox register
        ldr    x2, [x1, #0x18]
        ldr    x3, =exec_cpu_var
        str    w2, [x3]
        ret

init_mmu:
// Instead of running tests from EL2 if EL2 MPU regions exist, we run applications from
// EL1 to align with other v8 systems. If you'd like to run from EL2 when available, uncomment
// the following three lines. If you do, make sure to also update pmu_Av8.s to count PMU events
// for EL2 if desired.
//        mrs   x0, S3_4_C0_C0_4          // MPUIR_EL2 - If no EL2 MPU regions are found, switch to EL1 to run the test
//        cmp   x0, #0
//        b.eq  EL1_MPU_PROG
        b     EL1_MPU_PROG

//-------------------------------------------------------------------------------
// EL2 MPU setup
//-------------------------------------------------------------------------------

EL2_MPU_PROG:
        // MAIR_EL2 needs to be initialized with different attrindx
        //  MAIR_EL2.Attr0 - Non-cacheable
        //  MAIR_EL2.Attr1 - Write-back, RWA
        //  MAIR_EL2.Attr2 - Device nGnRnE
        mov     x0, #0xff44
        msr     MAIR_EL2, x0
        // MPU region 0 - executable with non-cacheable attributes
el2_mpu_rgn0:
        //                        MPU, region, base_addr,    sh,  ap,  xn,  limit_addr,   ns,  attr_index, reg_en, reg=0
        // MPU region for Main/I-TCM
        mpu_select_region         EL2, 0x0
        mpu_setup_region          EL2, 0x0,    0x0000000000, 0x2, 0x0, 0x0, 0x00000FFFFF, 0x0, 0x1,         0x1,    0
el2_mpu_rgn1:
        // MPU region for Main/D-TCM
        mpu_select_region         EL2, 0x1
        mpu_setup_region          EL2, 0x0,    0x0000100000, 0x2, 0x0, 0x0, 0x00001FFFFF, 0x0, 0x1,         0x1,    0
el2_mpu_rgn2:
        // MPU region for Global Data, Main (Cacheable)
        mpu_select_region         EL2, 0x2
        mpu_setup_region          EL2, 0x0,    0x0000200000, 0x2, 0x0, 0x0, 0x00002FFFFF, 0x0, 0x1,         0x1,    0
el2_mpu_rgn3:
        // MPU region for LLRAM
        mpu_select_region         EL2, 0x3
        mpu_setup_region          EL2, 0x0,    0x0030000000, 0x2, 0x0, 0x0, 0x003FFFFFFF, 0x0, 0x1,         0x1,    0
el2_mpu_rgn4:
       // MPU region for SPP
       mpu_select_region          EL2, 0x4
       mpu_setup_region           EL2, 0x0,    0x0040000000, 0x2, 0x0, 0x1, 0x0047FFFFFF, 0x0, 0x2,         0x1,    0
el2_mpu_rgn5:
       // MPU region for APB Debug Port
        mpu_select_region         EL2, 0x5
        mpu_setup_region          EL2, 0x0,    0x0050000000, 0x2, 0x0, 0x1, 0x0057FFFFFF, 0x0, 0x2,         0x1,    0
el2_mpu_rgn6:
       // MPU region for LLPP/Tube
       mpu_select_region          EL2, 0x6
       mpu_setup_region           EL2, 0x0,    0x00B0000000, 0x2, 0x0, 0x1, 0x00B7FFFFFF, 0x0, 0x2,         0x1,    0

sctlr_el2:
        // Enable the background region and caches
        mrs     x0, sctlr_el2
        orr     x0, x0, #(0x1 << 0)     // M  bit (MMU on)
        orr     x0, x0, #(0x1 << 2)     // C  bit (data cache)
        orr     x0, x0, #(0x1 << 12)    // I  bit (instruction cache)
        orr     x0, x0, #(0x1 << 17)    // BR bit (background region)
        msr     sctlr_el2, x0
        isb     sy
        b       test

//-------------------------------------------------------------------------------
// EL1 MPU setup
//-------------------------------------------------------------------------------

EL1_MPU_PROG:
        // MAIR_EL2 needs to be initialized with different attrindx
        //  MAIR_EL2.Attr0 - Non-cacheable
        //  MAIR_EL2.Attr1 - Write-back, RWA
        //  MAIR_EL2.Attr2 - Device nGnRnE
        mov     x0, #0xff44
        msr     MAIR_EL1, x0
        // MPU region 0 - executable with non-cacheable attributes
el1_mpu_rgn0:
        //                        MPU, region, base_addr,    sh,  ap,  xn,  limit_addr,   ns,  attr_index, reg_en, reg=0
        // MPU region for Main/I-TCM
        mpu_select_region         EL1, 0x0
        mpu_setup_region          EL1, 0x0,    0x0000000000, 0x2, 0x0, 0x0, 0x00000FFFFF, 0x0, 0x1,         0x1,    0
el1_mpu_rgn1:
        // MPU region for Main/D-TCM
        mpu_select_region         EL1, 0x1
        mpu_setup_region          EL1, 0x0,    0x0000100000, 0x2, 0x0, 0x0, 0x00001FFFFF, 0x0, 0x1,         0x1,    0
el1_mpu_rgn2:
        // MPU region for Main(Cacheable)/D-TCM
        mpu_select_region         EL1, 0x2
        mpu_setup_region          EL1, 0x0,    0x0000200000, 0x2, 0x0, 0x0, 0x00002FFFFF, 0x0, 0x1,         0x1,    0
el1_mpu_rgn3:
        // MPU region for LLRAM
        mpu_select_region         EL1, 0x3
        mpu_setup_region          EL1, 0x0,    0x0030000000, 0x2, 0x0, 0x0, 0x003FFFFFFF, 0x0, 0x1,         0x1,    0
el1_mpu_rgn4:
        // MPU region for SPP
        mpu_select_region         EL1, 0x4
        mpu_setup_region          EL1, 0x0,    0x0040000000, 0x2, 0x0, 0x1, 0x0047FFFFFF, 0x0, 0x2,         0x1,    0
el1_mpu_rgn5:
        // MPU region for APB Debug Port
        mpu_select_region         EL1, 0x5
        mpu_setup_region          EL1, 0x0,    0x0050000000, 0x2, 0x0, 0x1, 0x0057FFFFFF, 0x0, 0x2,         0x1,    0
el1_mpu_rgn6:
        // MPU region for LLPP/Tube
        mpu_select_region         EL1, 0x6
        mpu_setup_region          EL1, 0x0,    0x00B0000000, 0x2, 0x0, 0x1, 0x00B7FFFFFF, 0x0, 0x2,         0x1,    0

sctlr_el1:
        // Enable the background region and caches
        mrs     x0, sctlr_el1
        orr     x0, x0, #(0x1 << 0)     // M  bit (MMU on)
        orr     x0, x0, #(0x1 << 2)     // C  bit (data cache)
        orr     x0, x0, #(0x1 << 12)    // I  bit (instruction cache)
        orr     x0, x0, #(0x1 << 17)    // BR bit (background region)
        orr     x0, x0, #(0x1 << 23)    // SPAN bit (don't set PAN)
        orr     x0, x0, #(0x1 << 27)    // EnDA bit (Enable pointer authentication using APDAKey_EL1)
        msr     sctlr_el1, x0
        isb

        // Disable entering EL2 by setting HCR_EL2.HCD to 1
        mrs     x0, hcr_el2
        orr     x0, x0, #(0x1 << 29)    // Set HCD
        msr     hcr_el2, x0

        // Disable traps at EL1 for floating point accesses
        mrs     x0, cpacr_el1
        orr     x0, x0, #(0x1 << 20)    //CAPCR_EL1.FPEN[21:20] = 0b01 instructions accessing SVE trapped at EL0 but not at EL1
        msr     cpacr_el1, x0

       // Auxiliary register write control in ACTLR_EL2
       mrs      x0, actlr_el2
       orr      x0, x0, #(0x1 << 9)
       msr      actlr_el2, x0

        // Copy MPIDR_EL1 to VMPIDR_EL2, as VMPIDR_EL2 is read in EL1
        mrs     x0, mpidr_el1
        msr     vmpidr_el2, x0

       // Copy MIDR_EL1 to VMIDR_EL2, as VMPIDR_EL2 is read in EL1
        mrs     x0, midr_el1
        msr     vpidr_el2, x0

        // Ensure all writes to system registers have taken place
        // ISB should be sufficient
        isb     sy

        // Setup SPSR_EL2
        mov     x0, #5
        msr     spsr_el2, x0

        // Setup ELR_EL2 to branch to SUBMAIN
        .global     submain
        adr     x0, submain
        msr     elr_el2, x0
        eret

//-------------------------------------------------------------------------------
// Start the test
//-------------------------------------------------------------------------------

test:
        .weak test_start_mp
        .weak _arm_start_mp
        .weak _arm_start
        .weak test_start
        .type test_start, %function


        b       test_start_mp
        b       _arm_start_mp

      // Core synchronisation to ensure that all cores restart at the same time
       ldr     x0, =core_sync
       // bl      core_synchronisation

       // Only selected CPU starts the test code.  Other CPUs sleep and will be enabled
       // in individual tests where required

        mrs    x0, mpidr_el1
        and    x0, x0, #0xffff     // x0 == AFF1 and AFF0

        ldr    x1, =exec_cpu_var
        ldr    w2, [x1]

        cmp    w0, w2
        b.eq   cpu_start
        ldr    x0, =TUBE_ADDRESS
        ldr    x3, =wfi_message
        bl     print_wfi
        dsb    sy

wfi_loop:
        wfi
        b wfi_loop

cpu_start:
        .global     submain
        b           submain
        b       test_start // Set by ASM tests
        b       _arm_start // Used by C based tests


        .type print_wfi, %function
print_wfi:   // Uses w0 - tube address and x3 - message
        ldrb    w2, [x3], #1
        cmp     w2, #0x0
        beq     print_wfi_ret
        strb    w2, [x0]
        b       print_wfi

print_wfi_ret:
        ret

//-------------------------------------------------------------------------------
// Subroutines
//-------------------------------------------------------------------------------

        // Routine for core synchronisation to ensure
        // that all cores restart at the same time
        // return w1, The sequence number of the PE which incremented the sync variable
        //            last PE to enter will have this value = number of powered PEs
        // return w2, Total number of powered PEs
.type core_synchronisation, %function
core_synchronisation:

        // Check the total number of powered PEs in the Cluster
        mrs      x2, S3_0_c15_c3_0       // CLUSTERCFR
        ubfx     w2, w2, #0, #4          // Total number of cores
        // All the cores are powered up

        // Increment the sync variable to indicate this core is waiting
core_waiting:
        ldxr    w1, [x0]
        add     w1, w1, #1
        cmp     w1, w2
        b.ne    increment_sync

        // Last PE clears the sync variable for next use
        stxr    w3, wzr, [x0]
        cbnz    w3, core_waiting

        // Ensure core_sync write is observable by others
        dsb     sy

        b       core_synced

increment_sync:
        stxr    w3, w1, [x0]
        cbnz    w3, core_waiting

        // Ensure core_sync write is observable by others
        dsb     sy

last_pe:
        ldxr    w3, [x0]
        cmp     w3, wzr
        b.eq    core_synced
        wfe
        // If wakeup, check if the slowest core
        // has completed the initialization code
        b       last_pe

core_synced:
        ret

.type neon_fp_check, %function
neon_fp_check:
                // Read ID_AA64PFR0 to determine whether floating-point and
                // Advanced SIMD is implemented.  ID_AA64PFR0[19:16] shows
                // whether floating-point is implemented.
                mrs     x0, id_aa64pfr0_el1
                sbfx    w0, w0, #20, #4         // Bits[19:16]
                cmp     w0, #1
                b.ne    neonfp_end_test
                ret

neonfp_end_test:
                ldr     x1, =neon_fp_message
                b       neon_fp_end_test

.type neon_fp_end_test, %function
neon_fp_end_test:
                // Write the text one byte at a time to the tube address
                ldr     x0, =TUBE_ADDRESS // Tube address

print_loop:     ldrb    w2, [x1], #1
                cbz     w2, print_comp   // Branch to end on NULL byte
                strb    w2, [x0]
                b       print_loop

print_comp:
                dsb     sy
                ret

end_wfi:        wfi
                b       end_wfi

                ret

// This functions check if the executing core is Fuji ( kinabalu R0 Part ID = D15, Fuji Part ID = D14 )
.type is_fuji, %function
is_fuji:      mov     x12, #0
              mrs     x5, midr_el1
              ubfx    x5, x5, #4, #12
              cmp     w5, #0xD14
              b.eq    end_function
              mov     x12, #1

end_function: ret



// Data Local to each Core
        .section .data, "aw", %progbits
wfi_message:
        .asciz "Core sleeping\n"

        .balign 8
exec_cpu_var:
        .word 0

        .balign 4
neon_fp_message:
        .asciz "FATAL: Floating-point is not implemented for the selected EXEC_CPU\n\n** TEST SKIPPED **\n\4"

// Global Data
        .section globaldata, "aw", %progbits

        .balign 8
core_sync:
        .word 0
        .end
