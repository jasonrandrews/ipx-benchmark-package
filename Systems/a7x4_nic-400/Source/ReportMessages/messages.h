#ifndef _MESSAGES_H_
#define _MESSAGES_H_

// clang-format off

// General simulation information
#define FM_START_DATE_TIME  "FM start date time"
#define FM_STOP_DATE_TIME   "FM stop date time"
#define FM_TOTAL_INST       "Total number of instructions"

#define CM_START_DATE_TIME  "CM start date time"
#define CM_STOP_DATE_TIME   "CM stop date time"
#define CM_TOTAL_CYCLE      "Total number of cycles"
#define CM_TOTAL_CPS        "Total cycles per second"
#define CM_CUR_CYCLE        "Current cycle number"
#define CM_SIM_TIME         "Simulation duration"

// Boot code + Marked software code information
#define FM_BOOT_COUNT   "Boot code instruction count"
#define FM_CODE_START   "Marked software instruction count start"
#define FM_CODE_STOP    "Marked software instruction count stop"
#define FM_CODE_COUNT   "Marked software instruction count"

#define CM_BOOT_COUNT   "Boot code cycle count"
#define CM_CODE_START   "Marked software cycle count start"
#define CM_CODE_STOP    "Marked software cycle count stop"
#define CM_CODE_COUNT   "Marked software cycle count"

// Names for loggers whose output is routed to reporting channels.
#define AXI_RAM_LOGGER_NAME    "ram"
#define AHB_RAM_LOGGER_NAME    "ahb"

// Logger complete messages
#define AXI_LOGGER_NAME         "AXI logger complete"
#define AHB_LOGGER_NAME         "AHB logger complete"

// Common memory access information
#define BUS_CYCLE_COUNT             "Total cycles"
#define BUS_NRES_COUNT              "Non-reset cycles"
#define BUS_READ_COUNT              "Number of reads"
#define BUS_MAX_READ_LAT            "Max read latency"
#define BUS_MAX_READ_LAT_CYCLE      "Cycle number with max read latency"
#define BUS_WRITE_COUNT             "Number of writes"
#define BUS_MAX_WRITE_LAT           "Max write latency"
#define BUS_MAX_WRITE_LAT_CYCLE     "Cycle number with max write latency"

// AXI specific memory access information
#define AXI_AVE_READ_LAT            "Ave read latency"
#define AXI_AVE_READ_THROUGHPUT     "Ave read throughput"
#define AXI_READ_CHAN_UTIL          "Read channel utilization"
#define AXI_READ_BUS_UTIL           "Read bus utilization"
#define AXI_AVE_WRITE_LAT           "Ave write latency"
#define AXI_AVE_WRITE_THROUGHPUT    "Ave write throughput"
#define AXI_WRITE_CHAN_UTIL         "Write channel utilization"
#define AXI_WRITE_BUS_UTIL          "Write bus utilization"
#define AXI_AVE_LAT                 "Ave latency (read+write)"

// PMU software data point
#define PMU_START               "Marked software performance monitor results"
#define PMU_CCNT                "CCNT"
#define PMU_CPICNT              "CPICNT"
#define PMU_EXCCNT              "EXCCNT"
#define PMU_SLEEPCNT            "SLEEPCNT"
#define PMU_LSUCNT              "LSUCNT"
#define PMU_FOLDCNT             "FOLDCNT"
#define PMU_EXEC_INSTRS         "Instructions executed"
#define PMU_MEM_ACCESSES        "Memory accesses"
#define PMU_L1_DCACHE_REFILLS   "L1 Data cache refills"
#define PMU_L1_DCACHE_ACCESSES  "L1 Data cache accesses"
#define PMU_L1_DCACHE_MISSES    "L1 Data cache misses"
#define PMU_ICACHE_MISSES       "Instruction cache misses"
#define PMU_MEM_READS           "Memory reads"
#define PMU_AVE_CPI             "Average cycles per instruction"
#define PMU_END                 "End of marked software performance monitor results"

// clang-format on

#endif  // _MESSAGES_H_
