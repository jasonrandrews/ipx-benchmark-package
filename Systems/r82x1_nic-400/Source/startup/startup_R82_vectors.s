//=============================================================================
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from Arm Limited or its affiliates.
//
//             (C) COPYRIGHT 2013 - 2021 Arm Limited or its affiliates.
//                 ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from Arm Limited or its affiliates.
//
//     Release Information : Cortex-R82 Processor MP130-r0p1-00eac0
//
//=============================================================================

        .section vectors, "ax", %progbits
        #include "kinfu_execution_defs.hs"
        .global  vector_table

        // Weakly-import test-defined handlers.  The weak import means that if
        // a test does not define the handler then the default one will be
        // used.
        .weak curr_el_sp0_sync_vector
        .weak curr_el_sp0_irq_vector
        .weak curr_el_sp0_fiq_vector
        .weak curr_el_sp0_serror_vector
        .weak curr_el_spx_sync_vector
        .weak curr_el_spx_irq_vector
        .weak curr_el_spx_fiq_vector
        .weak curr_el_spx_serror_vector
        .weak lower_el_aarch64_sync_vector
        .weak lower_el_aarch64_irq_vector
        .weak lower_el_aarch64_fiq_vector
        .weak lower_el_aarch64_serror_vector


        // Start of vectors
        .balign 0x800

        // Current EL with SP0
vector_table:
curr_el_sp0_sync:
        // Branch to the weakly-imported test-defined handler.  If the test
        // does not define a handler then this instruction behaves like a NOP.
        b       curr_el_sp0_sync_vector

        // Default code when a test does not define a replacement handler
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_sp0_sync\n"

        // Ensure next vector is aligned to the correct boundary
        .balign 0x80
curr_el_sp0_irq:
        b       curr_el_sp0_irq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_sp0_irq\n"

        .balign 0x80
curr_el_sp0_fiq:
        b       curr_el_sp0_fiq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_sp0_fiq\n"

        .balign 0x80
curr_el_sp0_serror:
        b       curr_el_sp0_serror_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_sp0_serror\n"

        .balign 0x80
        // Current EL with SPx
curr_el_spx_sync:
        b       curr_el_spx_sync_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_spx_sync\n"

        .balign 0x80
curr_el_spx_irq:
        b       curr_el_spx_irq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_spx_irq\n"

        .balign 0x80
curr_el_spx_fiq:
        b       curr_el_spx_fiq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_spx_fiq\n"

        .balign 0x80
curr_el_spx_serror:
        b       curr_el_spx_serror_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: curr_el_spx_serror\n"

        .balign 0x80
        // Lower EL using AArch64
lower_el_aarch64_sync:
        b       lower_el_aarch64_sync_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: lower_el_aarch64_sync\n"

        .balign 0x80
lower_el_aarch64_irq:
        b       lower_el_aarch64_irq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: lower_el_aarch64_irq\n"

        .balign 0x80
lower_el_aarch64_fiq:
        b       lower_el_aarch64_fiq_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: lower_el_aarch64_fiq\n"

        .balign 0x80
lower_el_aarch64_serror:
        b       lower_el_aarch64_serror_vector
        ldr     x0, =TUBE_ADDRESS // Tube address
        adr     x1, 1f
        bl      print
        b       terminate

1:      .asciz "Unexpected exception: lower_el_aarch64_serror\n"

        .balign 0x80

//------------------------------------------------------------------------------
// End of vectors
//------------------------------------------------------------------------------

// Print a string to the tube
//    Expects: x0 -> tube
//             x1 -> message
//    Modifies x2
print:          ldrb    w2, [x1], #1
                cbz     w2, 1f
                strb    w2, [x0]
                b       print
1:              ret

// Print failure message and terminate simulation.
// Assumes x0 points to the tube
terminate:      adr     x1, fail_str
                bl      print
                mov     w2, #0x11  // Test Fail character
                strb    w2, [x0]   // Terminate simulation
                b       .

// Failure string for unexpected exceptions
                .balign 4
fail_str:       .asciz "** TEST FAILED**\n"
                .end
