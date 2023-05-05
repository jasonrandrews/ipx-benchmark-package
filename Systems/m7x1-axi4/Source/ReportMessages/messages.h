/*********************************************************************/
/*      Copyright (c) 2021-2022 Arm Limited. All rights reserved.    */
/*********************************************************************/

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

// clang-format off

// Boot code + Marked software code information
#define FM_BOOT_COUNT   "Boot code instruction count"
#define FM_CODE_START   "Marked software start instruction count"
#define FM_CODE_STOP    "Marked software stop instruction count"
#define FM_CODE_RESUME  "Marked software resume instruction count"
#define FM_CODE_PAUSE   "Marked software pause instruction count"
#define FM_CODE_COUNT   "Marked software total instruction count"

// PMU software data point
#define PMU_START               "Marked software performance monitor results"
#define PMU_CCNT                "CCNT"
#define PMU_CPICNT              "CPICNT"
#define PMU_EXCCNT              "EXCCNT"
#define PMU_SLEEPCNT            "SLEEPCNT"
#define PMU_LSUCNT              "LSUCNT"
#define PMU_FOLDCNT             "FOLDCNT"
#define PMU_EXEC_INSTRS         "Instructions executed"
#define PMU_MVE_EXEC_INSTRS     "MVE Instructions executed"
#define PMU_MEM_ACCESSES        "Memory accesses"
#define PMU_DTCM_ACCESSES       "DTCM accesses"
#define PMU_L1_DCACHE_REFILLS   "L1 Data cache refills"
#define PMU_L1_DCACHE_ACCESSES  "L1 Data cache accesses"
#define PMU_L1_DCACHE_MISSES    "L1 Data cache misses"
#define PMU_ICACHE_MISSES       "Instruction cache misses"
#define PMU_MEM_READS           "Memory reads"
#define PMU_AVE_CPI             "Average cycles per instruction"
#define PMU_END                 "End of marked software performance monitor results"

// clang-format on

#endif  // _MESSAGES_H_
