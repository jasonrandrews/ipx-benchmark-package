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
// Purpose:
//   This file contains definitions that are used by multiple files.  It is
//   loaded by files that require these symbols using the "#include" directive.
//   The constants defined here are non-modifiable.
//-----------------------------------------------------------------------------

// Stack definitions.  Each CPU gets a chunk of the global stack space. This
// is only used by the bootcode to setup stack and heap before the test
// library is called. These constants do not affect the production code
.equ CPU_STACK_SIZE     ,      1024
.equ STACK_SIZE         ,      CPU_STACK_SIZE*4
.equ HEAP_SIZE          ,      1024*8

// Tube, output to terminal
.equ TUBE_ADDRESS       ,      0x00B0000000

// PPU REG OFFSET
.equ PPU_CLUSTER_OFFSET ,      0x0040010008
.equ PPU_CORE_OFFSET    ,      0x0040040008
.equ DEBUG_APB_ADDRESS  ,      0x0050000000

