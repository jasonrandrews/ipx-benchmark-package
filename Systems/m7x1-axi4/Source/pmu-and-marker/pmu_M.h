/*********************************************************************/
/*        Copyright (c) 2020,2022 Arm Limited. All rights reserved.  */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/
/* PMU variables for Cortex-M Cores */

#ifndef __PMU_M_H__
#define __PMU_M_H__

// Choose the correct header for the compiler
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#include "ipss_armclang.h"
#elif defined(__GNUC__)
#include "ipss_gcc.h"
#endif

#include <stdint.h>

void init_dwt(void);
void start_dwt(void);
void stop_dwt(void);
void start_systick(void);
void stop_systick(void);

/* DWT Variables */
#define CM_DWT_CYCCNTENA_BIT (1UL << 0)
#define CM_DWT_CPIEVTENA_BIT (1UL << 17)
#define CM_DWT_EXCEVTENA_BIT (1UL << 18)
#define CM_DWT_SLEEPEVTENA_BIT (1UL << 19)
#define CM_DWT_LSUEVTENA_BIT (1UL << 20)
#define CM_DWT_FOLDEVTENA_BIT (1UL << 21)
#define CM_TRCENA_BIT (1UL << 24)

#define CM_DWT_CONTROL (*((volatile uint32_t*)0xE0001000))
#define CM_DWT_CYCCNT (*((volatile uint32_t*)0xE0001004))
#define CM_DWT_LAR (*((volatile uint32_t*)0xE0001FB0))
#define CM_DEMCR (*((volatile uint32_t*)0xE000EDFC))

#define CM_DWT_CPICNT (*((volatile uint32_t*)0xE0001008))
#define CM_DWT_EXCCNT (*((volatile uint32_t*)0xE000100C))
#define CM_DWT_SLEEPCNT (*((volatile uint32_t*)0xE0001010))
#define CM_DWT_LSUCNT (*((volatile uint32_t*)0xE0001014))
#define CM_DWT_FOLDCNT (*((volatile uint32_t*)0xE0001018))

/* Systick variables */
#define SysTick_BASE (0xE000E000UL + 0x0010UL)
#define SysTick_START 0xFFFFFF

#define SysTick_CSR (*((volatile uint32_t*)(SysTick_BASE + 0x0UL)))
#define SysTick_RVR (*((volatile uint32_t*)(SysTick_BASE + 0x4UL)))
#define SysTick_CVR (*((volatile uint32_t*)(SysTick_BASE + 0x8UL)))

#define SysTick_Enable 0x1
#define SysTick_ClockSource 0x4

/************************************************************************
The rest of this file is PMU specific code, adapted from the
CMSIS implementation.
Applies only to Arm V8.1-M CPUs.
*************************************************************************/
#define     __IOM    volatile  /*! Defines 'read / write' structure member permissions */

// Currently, all V8.1-M CPUs have 8 event counters
// If that changes in the future, this variable will need to be updated
#define __PMU_NUM_EVENTCNT      8U

// Structure type to access the Performance Monitoring Unit (PMU).
typedef struct
{
  __IOM uint32_t EVCNTR[__PMU_NUM_EVENTCNT];        /*!< Offset: 0x0 (R/W)    PMU Event Counter Registers */
#if __PMU_NUM_EVENTCNT<31
        uint32_t RESERVED0[31U-__PMU_NUM_EVENTCNT];
#endif
  __IOM uint32_t CCNTR;                             /*!< Offset: 0x7C (R/W)   PMU Cycle Counter Register */
        uint32_t RESERVED1[224];
  __IOM uint32_t EVTYPER[__PMU_NUM_EVENTCNT];       /*!< Offset: 0x400 (R/W)  PMU Event Type and Filter Registers */
#if __PMU_NUM_EVENTCNT<31
        uint32_t RESERVED2[31U-__PMU_NUM_EVENTCNT];
#endif
  __IOM uint32_t CCFILTR;                           /*!< Offset: 0x47C (R/W)  PMU Cycle Counter Filter Register */
        uint32_t RESERVED3[480];
  __IOM uint32_t CNTENSET;                          /*!< Offset: 0xC00 (R/W)  PMU Count Enable Set Register */
        uint32_t RESERVED4[7];
  __IOM uint32_t CNTENCLR;                          /*!< Offset: 0xC20 (R/W)  PMU Count Enable Clear Register */
        uint32_t RESERVED5[7];
  __IOM uint32_t INTENSET;                          /*!< Offset: 0xC40 (R/W)  PMU Interrupt Enable Set Register */
        uint32_t RESERVED6[7];
  __IOM uint32_t INTENCLR;                          /*!< Offset: 0xC60 (R/W)  PMU Interrupt Enable Clear Register */
        uint32_t RESERVED7[7];
  __IOM uint32_t OVSCLR;                            /*!< Offset: 0xC80 (R/W)  PMU Overflow Flag Status Clear Register */
        uint32_t RESERVED8[7];
  __IOM uint32_t SWINC;                             /*!< Offset: 0xCA0 (R/W)  PMU Software Increment Register */
        uint32_t RESERVED9[7];
  __IOM uint32_t OVSSET;                            /*!< Offset: 0xCC0 (R/W)  PMU Overflow Flag Status Set Register */
        uint32_t RESERVED10[79];
  __IOM uint32_t TYPE;                              /*!< Offset: 0xE00 (R/W)  PMU Type Register */
  __IOM uint32_t CTRL;                              /*!< Offset: 0xE04 (R/W)  PMU Control Register */
        uint32_t RESERVED11[108];
  __IOM uint32_t AUTHSTATUS;                        /*!< Offset: 0xFB8 (R/W)  PMU Authentication Status Register */
  __IOM uint32_t DEVARCH;                           /*!< Offset: 0xFBC (R/W)  PMU Device Architecture Register */
        uint32_t RESERVED12[4];
  __IOM uint32_t DEVTYPE;                           /*!< Offset: 0xFCC (R/W)  PMU Device Type Register */
  __IOM uint32_t PIDR4;                             /*!< Offset: 0xFD0 (R/W)  PMU Peripheral Identification Register 4 */
        uint32_t RESERVED13[3];
  __IOM uint32_t PIDR0;                             /*!< Offset: 0xFE0 (R/W)  PMU Peripheral Identification Register 0 */
  __IOM uint32_t PIDR1;                             /*!< Offset: 0xFE0 (R/W)  PMU Peripheral Identification Register 1 */
  __IOM uint32_t PIDR2;                             /*!< Offset: 0xFE0 (R/W)  PMU Peripheral Identification Register 2 */
  __IOM uint32_t PIDR3;                             /*!< Offset: 0xFE0 (R/W)  PMU Peripheral Identification Register 3 */
        uint32_t RESERVED14[3];
  __IOM uint32_t CIDR0;                             /*!< Offset: 0xFF0 (R/W)  PMU Component Identification Register 0 */
  __IOM uint32_t CIDR1;                             /*!< Offset: 0xFF4 (R/W)  PMU Component Identification Register 1 */
  __IOM uint32_t CIDR2;                             /*!< Offset: 0xFF8 (R/W)  PMU Component Identification Register 2 */
  __IOM uint32_t CIDR3;                             /*!< Offset: 0xFFC (R/W)  PMU Component Identification Register 3 */
} PMU_Type;

#define PMU_BASE          (0xE0003000UL)         /*!< PMU Base Address */
#define PMU               ((PMU_Type*) PMU_BASE) /*!< PMU configuration struct */

//PMU Event Counter Registers (0-30) Definitions
#define PMU_EVCNTR_CNT_Pos                    0U                                           /*!< PMU EVCNTR: Counter Position */
#define PMU_EVCNTR_CNT_Msk                   (0xFFFFUL /*<< PMU_EVCNTRx_CNT_Pos*/)         /*!< PMU EVCNTR: Counter Mask */

// PMU Count Enable Set Register Definitions
#define PMU_CNTENSET_CNT0_ENABLE_Pos          0U                                           /*!< PMU CNTENSET: Event Counter 0 Enable Set Position */
#define PMU_CNTENSET_CNT0_ENABLE_Msk         (1UL /*<< PMU_CNTENSET_CNT0_ENABLE_Pos*/)     /*!< PMU CNTENSET: Event Counter 0 Enable Set Mask */

#define PMU_CNTENSET_CNT1_ENABLE_Pos          1U                                           /*!< PMU CNTENSET: Event Counter 1 Enable Set Position */
#define PMU_CNTENSET_CNT1_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT1_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 1 Enable Set Mask */

#define PMU_CNTENSET_CNT2_ENABLE_Pos          2U                                           /*!< PMU CNTENSET: Event Counter 2 Enable Set Position */
#define PMU_CNTENSET_CNT2_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT2_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 2 Enable Set Mask */

#define PMU_CNTENSET_CNT3_ENABLE_Pos          3U                                           /*!< PMU CNTENSET: Event Counter 3 Enable Set Position */
#define PMU_CNTENSET_CNT3_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT3_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 3 Enable Set Mask */

#define PMU_CNTENSET_CNT4_ENABLE_Pos          4U                                           /*!< PMU CNTENSET: Event Counter 4 Enable Set Position */
#define PMU_CNTENSET_CNT4_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT4_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 4 Enable Set Mask */

#define PMU_CNTENSET_CNT5_ENABLE_Pos          5U                                           /*!< PMU CNTENSET: Event Counter 5 Enable Set Position */
#define PMU_CNTENSET_CNT5_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT5_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 5 Enable Set Mask */

#define PMU_CNTENSET_CNT6_ENABLE_Pos          6U                                           /*!< PMU CNTENSET: Event Counter 6 Enable Set Position */
#define PMU_CNTENSET_CNT6_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT6_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 6 Enable Set Mask */

#define PMU_CNTENSET_CNT7_ENABLE_Pos          7U                                           /*!< PMU CNTENSET: Event Counter 7 Enable Set Position */
#define PMU_CNTENSET_CNT7_ENABLE_Msk         (1UL << PMU_CNTENSET_CNT7_ENABLE_Pos)         /*!< PMU CNTENSET: Event Counter 7 Enable Set Mask */

#define PMU_CNTENSET_CCNTR_ENABLE_Pos         31U                                          /*!< PMU CNTENSET: Cycle Counter Enable Set Position */
#define PMU_CNTENSET_CCNTR_ENABLE_Msk        (1UL << PMU_CNTENSET_CCNTR_ENABLE_Pos)        /*!< PMU CNTENSET: Cycle Counter Enable Set Mask */

// PMU Count Enable Clear Register Definitions
#define PMU_CNTENSET_CNT0_ENABLE_Pos          0U                                           /*!< PMU CNTENCLR: Event Counter 0 Enable Clear Position */
#define PMU_CNTENCLR_CNT0_ENABLE_Msk         (1UL /*<< PMU_CNTENCLR_CNT0_ENABLE_Pos*/)     /*!< PMU CNTENCLR: Event Counter 0 Enable Clear Mask */

#define PMU_CNTENCLR_CNT1_ENABLE_Pos          1U                                           /*!< PMU CNTENCLR: Event Counter 1 Enable Clear Position */
#define PMU_CNTENCLR_CNT1_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT1_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 1 Enable Clear */

#define PMU_CNTENCLR_CNT2_ENABLE_Pos          2U                                           /*!< PMU CNTENCLR: Event Counter 2 Enable Clear Position */
#define PMU_CNTENCLR_CNT2_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT2_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 2 Enable Clear Mask */

#define PMU_CNTENCLR_CNT3_ENABLE_Pos          3U                                           /*!< PMU CNTENCLR: Event Counter 3 Enable Clear Position */
#define PMU_CNTENCLR_CNT3_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT3_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 3 Enable Clear Mask */

#define PMU_CNTENCLR_CNT4_ENABLE_Pos          4U                                           /*!< PMU CNTENCLR: Event Counter 4 Enable Clear Position */
#define PMU_CNTENCLR_CNT4_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT4_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 4 Enable Clear Mask */

#define PMU_CNTENCLR_CNT5_ENABLE_Pos          5U                                           /*!< PMU CNTENCLR: Event Counter 5 Enable Clear Position */
#define PMU_CNTENCLR_CNT5_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT5_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 5 Enable Clear Mask */

#define PMU_CNTENCLR_CNT6_ENABLE_Pos          6U                                           /*!< PMU CNTENCLR: Event Counter 6 Enable Clear Position */
#define PMU_CNTENCLR_CNT6_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT6_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 6 Enable Clear Mask */

#define PMU_CNTENCLR_CNT7_ENABLE_Pos          7U                                           /*!< PMU CNTENCLR: Event Counter 7 Enable Clear Position */
#define PMU_CNTENCLR_CNT7_ENABLE_Msk         (1UL << PMU_CNTENCLR_CNT7_ENABLE_Pos)         /*!< PMU CNTENCLR: Event Counter 7 Enable Clear Mask */

#define PMU_CNTENCLR_CCNTR_ENABLE_Pos         31U                                          /*!< PMU CNTENCLR: Cycle Counter Enable Clear Position */
#define PMU_CNTENCLR_CCNTR_ENABLE_Msk        (1UL << PMU_CNTENCLR_CCNTR_ENABLE_Pos)        /*!< PMU CNTENCLR: Cycle Counter Enable Clear Mask */

// PMU Control Register Definitions
#define PMU_CTRL_ENABLE_Pos                   0U                                           /*!< PMU CTRL: ENABLE Position */
#define PMU_CTRL_ENABLE_Msk                  (1UL /*<< PMU_CTRL_ENABLE_Pos*/)              /*!< PMU CTRL: ENABLE Mask */
#define PMU_CTRL_EVENTCNT_RESET_Pos           1U                                           /*!< PMU CTRL: Event Counter Reset Position */
#define PMU_CTRL_EVENTCNT_RESET_Msk          (1UL << PMU_CTRL_EVENTCNT_RESET_Pos)          /*!< PMU CTRL: Event Counter Reset Mask */
#define PMU_CTRL_CYCCNT_RESET_Pos             2U                                           /*!< PMU CTRL: Cycle Counter Reset Position */
#define PMU_CTRL_CYCCNT_RESET_Msk            (1UL << PMU_CTRL_CYCCNT_RESET_Pos)            /*!< PMU CTRL: Cycle Counter Reset Mask */

// List of PMU events for Arm V8.1-M
#define ARM_PMU_SW_INCR                              0x0000   /*!< Software update to the PMU_SWINC register, architecturally executed and condition code check pass */
#define ARM_PMU_L1I_CACHE_REFILL                     0x0001   /*!< L1 I-Cache refill */
#define ARM_PMU_L1D_CACHE_REFILL                     0x0003   /*!< L1 D-Cache refill */
#define ARM_PMU_L1D_CACHE                            0x0004   /*!< L1 D-Cache access */
#define ARM_PMU_LD_RETIRED                           0x0006   /*!< Memory-reading instruction architecturally executed and condition code check pass */
#define ARM_PMU_ST_RETIRED                           0x0007   /*!< Memory-writing instruction architecturally executed and condition code check pass */
#define ARM_PMU_INST_RETIRED                         0x0008   /*!< Instruction architecturally executed */
#define ARM_PMU_EXC_TAKEN                            0x0009   /*!< Exception entry */
#define ARM_PMU_EXC_RETURN                           0x000A   /*!< Exception return instruction architecturally executed and the condition code check pass */
#define ARM_PMU_PC_WRITE_RETIRED                     0x000C   /*!< Software change to the Program Counter (PC). Instruction is architecturally executed and condition code check pass */
#define ARM_PMU_BR_IMMED_RETIRED                     0x000D   /*!< Immediate branch architecturally executed */
#define ARM_PMU_BR_RETURN_RETIRED                    0x000E   /*!< Function return instruction architecturally executed and the condition code check pass */
#define ARM_PMU_UNALIGNED_LDST_RETIRED               0x000F   /*!< Unaligned memory memory-reading or memory-writing instruction architecturally executed and condition code check pass */
#define ARM_PMU_BR_MIS_PRED                          0x0010   /*!< Mispredicted or not predicted branch speculatively executed */
#define ARM_PMU_CPU_CYCLES                           0x0011   /*!< Cycle */
#define ARM_PMU_BR_PRED                              0x0012   /*!< Predictable branch speculatively executed */
#define ARM_PMU_MEM_ACCESS                           0x0013   /*!< Data memory access */
#define ARM_PMU_L1I_CACHE                            0x0014   /*!< Level 1 instruction cache access */
#define ARM_PMU_L1D_CACHE_WB                         0x0015   /*!< Level 1 data cache write-back */
#define ARM_PMU_BUS_ACCESS                           0x0019   /*!< Bus access */
#define ARM_PMU_MEMORY_ERROR                         0x001A   /*!< Local memory error */
#define ARM_PMU_BUS_CYCLES                           0x001D   /*!< Bus cycles */
#define ARM_PMU_CHAIN                                0x001E   /*!< For an odd numbered counter, increment when an overflow occurs on the preceding even-numbered counter on the same PE */
#define ARM_PMU_BR_RETIRED                           0x0021   /*!< Branch instruction architecturally executed */
#define ARM_PMU_BR_MIS_PRED_RETIRED                  0x0022   /*!< Mispredicted branch instruction architecturally executed */
#define ARM_PMU_STALL_FRONTEND                       0x0023   /*!< No operation issued because of the frontend */
#define ARM_PMU_STALL_BACKEND                        0x0024   /*!< No operation issued because of the backend */
#define ARM_PMU_LL_CACHE_RD                          0x0036   /*!< Last level data cache read */
#define ARM_PMU_LL_CACHE_MISS_RD                     0x0037   /*!< Last level data cache read miss */
#define ARM_PMU_L1D_CACHE_MISS_RD                    0x0039   /*!< Level 1 data cache read miss */
#define ARM_PMU_STALL                                0x003C   /*!< Stall cycle for instruction or operation not sent for execution */
#define ARM_PMU_L1D_CACHE_RD                         0x0040   /*!< Level 1 data cache read */
#define ARM_PMU_LE_RETIRED                           0x0100   /*!< Loop end instruction executed */
#define ARM_PMU_LE_CANCEL                            0x0108   /*!< Loop end instruction not taken */
#define ARM_PMU_SE_CALL_S                            0x0114   /*!< Call to secure function, resulting in Security state change */
#define ARM_PMU_SE_CALL_NS                           0x0115   /*!< Call to non-secure function, resulting in Security state change */
#define ARM_PMU_DWT_CMPMATCH0                        0x0118   /*!< DWT comparator 0 match */
#define ARM_PMU_DWT_CMPMATCH1                        0x0119   /*!< DWT comparator 1 match */
#define ARM_PMU_DWT_CMPMATCH2                        0x011A   /*!< DWT comparator 2 match */
#define ARM_PMU_DWT_CMPMATCH3                        0x011B   /*!< DWT comparator 3 match */
#define ARM_PMU_MVE_INST_RETIRED                     0x0200   /*!< MVE instruction architecturally executed */
#define ARM_PMU_MVE_FP_RETIRED                       0x0204   /*!< MVE floating-point instruction architecturally executed */
#define ARM_PMU_MVE_FP_HP_RETIRED                    0x0208   /*!< MVE half-precision floating-point instruction architecturally executed */
#define ARM_PMU_MVE_FP_SP_RETIRED                    0x020C   /*!< MVE single-precision floating-point instruction architecturally executed */
#define ARM_PMU_MVE_FP_MAC_RETIRED                   0x0214   /*!< MVE floating-point multiply or multiply-accumulate instruction architecturally executed */
#define ARM_PMU_MVE_INT_RETIRED                      0x0224   /*!< MVE integer instruction architecturally executed */
#define ARM_PMU_MVE_INT_MAC_RETIRED                  0x0228   /*!< MVE multiply or multiply-accumulate instruction architecturally executed */
#define ARM_PMU_MVE_LDST_RETIRED                     0x0238   /*!< MVE load or store instruction architecturally executed */
#define ARM_PMU_MVE_LD_RETIRED                       0x023C   /*!< MVE load instruction architecturally executed */
#define ARM_PMU_MVE_ST_RETIRED                       0x0240   /*!< MVE store instruction architecturally executed */
#define ARM_PMU_MVE_LDST_CONTIG_RETIRED              0x0244   /*!< MVE contiguous load or store instruction architecturally executed */
#define ARM_PMU_MVE_LD_CONTIG_RETIRED                0x0248   /*!< MVE contiguous load instruction architecturally executed */
#define ARM_PMU_MVE_ST_CONTIG_RETIRED                0x024C   /*!< MVE contiguous store instruction architecturally executed */
#define ARM_PMU_MVE_LDST_NONCONTIG_RETIRED           0x0250   /*!< MVE non-contiguous load or store instruction architecturally executed */
#define ARM_PMU_MVE_LD_NONCONTIG_RETIRED             0x0254   /*!< MVE non-contiguous load instruction architecturally executed */
#define ARM_PMU_MVE_ST_NONCONTIG_RETIRED             0x0258   /*!< MVE non-contiguous store instruction architecturally executed */
#define ARM_PMU_MVE_LDST_MULTI_RETIRED               0x025C   /*!< MVE memory instruction targeting multiple registers architecturally executed */
#define ARM_PMU_MVE_LD_MULTI_RETIRED                 0x0260   /*!< MVE memory load instruction targeting multiple registers architecturally executed */
#define ARM_PMU_MVE_ST_MULTI_RETIRED                 0x0264   /*!< MVE memory store instruction targeting multiple registers architecturally executed */
#define ARM_PMU_MVE_LDST_UNALIGNED_RETIRED           0x028C   /*!< MVE unaligned memory load or store instruction architecturally executed */
#define ARM_PMU_MVE_LD_UNALIGNED_RETIRED             0x0290   /*!< MVE unaligned load instruction architecturally executed */
#define ARM_PMU_MVE_ST_UNALIGNED_RETIRED             0x0294   /*!< MVE unaligned store instruction architecturally executed */
#define ARM_PMU_MVE_LDST_UNALIGNED_NONCONTIG_RETIRED 0x0298   /*!< MVE unaligned noncontiguous load or store instruction architecturally executed */
#define ARM_PMU_MVE_VREDUCE_RETIRED                  0x02A0   /*!< MVE vector reduction instruction architecturally executed */
#define ARM_PMU_MVE_VREDUCE_FP_RETIRED               0x02A4   /*!< MVE floating-point vector reduction instruction architecturally executed */
#define ARM_PMU_MVE_VREDUCE_INT_RETIRED              0x02A8   /*!< MVE integer vector reduction instruction architecturally executed */
#define ARM_PMU_MVE_PRED                             0x02B8   /*!< Cycles where one or more predicated beats architecturally executed */
#define ARM_PMU_MVE_STALL                            0x02CC   /*!< Stall cycles caused by an MVE instruction */
#define ARM_PMU_MVE_STALL_RESOURCE                   0x02CD   /*!< Stall cycles caused by an MVE instruction because of resource conflicts */
#define ARM_PMU_MVE_STALL_RESOURCE_MEM               0x02CE   /*!< Stall cycles caused by an MVE instruction because of memory resource conflicts */
#define ARM_PMU_MVE_STALL_RESOURCE_FP                0x02CF   /*!< Stall cycles caused by an MVE instruction because of floating-point resource conflicts */
#define ARM_PMU_MVE_STALL_RESOURCE_INT               0x02D0   /*!< Stall cycles caused by an MVE instruction because of integer resource conflicts */
#define ARM_PMU_MVE_STALL_BREAK                      0x02D3   /*!< Stall cycles caused by an MVE chain break */
#define ARM_PMU_MVE_STALL_DEPENDENCY                 0x02D4   /*!< Stall cycles caused by MVE register dependency */
#define ARM_PMU_ITCM_ACCESS                          0x4007   /*!< Instruction TCM access */
#define ARM_PMU_DTCM_ACCESS                          0x4008   /*!< Data TCM access */

// PMU Methods
// These are only applicable to V8.1 Cortex-M CPUs
void init_pmu(void);
__STATIC_INLINE void start_pmu(void);
__STATIC_INLINE void stop_pmu(void);

// Declarations for PMU Helper functions for V8.1-M CPUs
__STATIC_INLINE uint32_t ARM_PMU_Get_CCNTR(void);
__STATIC_INLINE uint32_t ARM_PMU_Get_EVCNTR(uint32_t num);
__STATIC_INLINE uint32_t ARM_PMU_Get_Chain_EVCNTR(uint32_t num);
__STATIC_INLINE void ARM_PMU_CNTR_Enable(uint32_t mask);
__STATIC_INLINE void ARM_PMU_CNTR_Disable(uint32_t mask);

// The following functions are defined here in the header so
// they can be inlined

// Function to start the PMU counters
__STATIC_INLINE void start_pmu() {
  // Enable the cycle counter and event counters
  ARM_PMU_CNTR_Enable(PMU_CNTENSET_CCNTR_ENABLE_Msk |
                      PMU_CNTENSET_CNT0_ENABLE_Msk |
                      PMU_CNTENSET_CNT1_ENABLE_Msk |
                      PMU_CNTENSET_CNT2_ENABLE_Msk |
                      PMU_CNTENSET_CNT3_ENABLE_Msk |
                      PMU_CNTENSET_CNT4_ENABLE_Msk |
                      PMU_CNTENSET_CNT5_ENABLE_Msk |
                      PMU_CNTENSET_CNT6_ENABLE_Msk |
                      PMU_CNTENSET_CNT7_ENABLE_Msk
                      );
}

// Function to stop the PMU counters
__STATIC_INLINE void stop_pmu() {
  // Disable the cycle counter and event counters
  ARM_PMU_CNTR_Disable(PMU_CNTENCLR_CCNTR_ENABLE_Msk |
                      PMU_CNTENCLR_CNT0_ENABLE_Msk |
                      PMU_CNTENCLR_CNT1_ENABLE_Msk |
                      PMU_CNTENCLR_CNT2_ENABLE_Msk |
                      PMU_CNTENCLR_CNT3_ENABLE_Msk |
                      PMU_CNTENCLR_CNT4_ENABLE_Msk |
                      PMU_CNTENCLR_CNT5_ENABLE_Msk |
                      PMU_CNTENCLR_CNT6_ENABLE_Msk |
                      PMU_CNTENCLR_CNT7_ENABLE_Msk
                      );
}

// Read cycle counter
__STATIC_INLINE uint32_t ARM_PMU_Get_CCNTR(void) {
  return PMU->CCNTR;
}

// Read given event counter
__STATIC_INLINE uint32_t ARM_PMU_Get_EVCNTR(uint32_t num) {
  return PMU_EVCNTR_CNT_Msk & PMU->EVCNTR[num];
}

// Read an event counter that has been chained with the n+1 counter to create
// a combined 32bit counter
__STATIC_INLINE uint32_t ARM_PMU_Get_Chain_EVCNTR(uint32_t num) {
  return (ARM_PMU_Get_EVCNTR(num + 1) << 16 ) | ( PMU_EVCNTR_CNT_Msk & ARM_PMU_Get_EVCNTR(num));
}

// Enable counters specified in mask
__STATIC_INLINE void ARM_PMU_CNTR_Enable(uint32_t mask) {
  PMU->CNTENSET = mask;
}

// Disable counters specified in mask
__STATIC_INLINE void ARM_PMU_CNTR_Disable(uint32_t mask) {
  PMU->CNTENCLR = mask;
}

#endif  // __PMU_M_H__
