//=============================================================================
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from Arm Limited or its affiliates.
//
//             (C) COPYRIGHT 2015 - 2021 Arm Limited or its affiliates.
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

// Load stack definitions

//-------------------------------------------------------------------------------
// Stack and heap area
//-------------------------------------------------------------------------------

        .global  stack_top
        .global  heap_base
        .global  heap_limit

        .section heap, "aw", %progbits
        #include "kinfu_execution_defs.hs"
heap_base:      .space   4
                .space   (HEAP_SIZE-8)
heap_limit:     .space   4

        .section stack, "aw", %progbits
        .align 12
        #include "kinfu_execution_defs.hs"
        .space   (STACK_SIZE-4)
stack_top:      .space   4

        .end
