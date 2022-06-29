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
//-----------------------------------------------------------------------------

.equ PRSELR_EL2, 0x000c6220
.equ PRBAR_EL2,  0x000c6800
.equ PRLAR_EL2,  0x000c6820
.equ PRSELR_EL1, 0x00086220
.equ PRBAR_EL1,  0x00086800
.equ PRLAR_EL1,  0x00086820

.equ MSR_BASE,   0xd5100000
.equ MRS_BASE,   0xd5300000

.macro enc_msr sysreg, reg=0
    .inst (MSR_BASE | \sysreg | \reg)
.endm

.macro enc_mrs sysreg, reg=0
    .inst (MRS_BASE | \sysreg | \reg)
.endm

.macro mpu_select_region mpu, region, reg=0
    mov     x\reg,       #\region
    .ifc \mpu, EL2
    enc_msr PRSELR_EL2, \reg
    .else
    enc_msr PRSELR_EL1, \reg
    .endif
    isb
.endm

.macro mpu_setup_region mpu, region, base_addr, sh, ap, xn, limit_addr, ns, attr_index, reg_en, reg=0
    .set _base, \base_addr & 0xffffffffffc0
    .set _limit, \limit_addr & 0xffffffffffc0

    ldr     x\reg, =_base
    .if \sh
    orr     x\reg, x\reg, #(\sh        << 4)
    .endif
    .if \ap
    orr     x\reg, x\reg, #(\ap        << 2)
    .endif
    .if \xn
    orr     x\reg, x\reg, #(\xn        << 0)
    .endif

    .ifc \mpu, EL2
    enc_msr (PRBAR_EL2 | ((\region & 0xf) << 7)), \reg
    .else
    enc_msr (PRBAR_EL1 | ((\region & 0xf) << 7)), \reg
    .endif

    ldr     x\reg, =_limit
    .if \ns
    orr     x\reg, x\reg, #(\ns         << 4)
    .endif
    .if \attr_index
    orr     x\reg, x\reg, #(\attr_index << 1)
    .endif
    .if \reg_en
    orr     x\reg, x\reg, #(\reg_en     << 0)
    .endif

    .ifc \mpu, EL2
    enc_msr (PRLAR_EL2 | ((\region & 0xf) << 7)), \reg
    .else
    enc_msr (PRLAR_EL1 | ((\region & 0xf) << 7)), \reg
    .endif

.endm
