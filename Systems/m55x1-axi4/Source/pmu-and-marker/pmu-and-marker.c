/*********************************************************************/
/*        Copyright (c) 2020-2022 Arm Limited. All rights reserved.  */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/
/* PMU functionality for all systems */

#include "ReportMessages/messages.h"
#include "pmu-and-marker.h"
#include "serial.h"
#include <stdio.h>
#include <inttypes.h>

#include "pmu_M.h"





unsigned int start = 0, stop = 0;


//Start tracking pmu metrics for region of interest
void start_marker() {
init_pmu();
/* Memory write to indicate software region of interest is starting
     This should be written after PMU/counters have been initialized
     but before the counters have been enabled
  */
  SOFTWARE_MARK = 1;

start_pmu();
}

//Pause tracking pmu metrics for region of interest
void pause_marker() {
  SOFTWARE_MARK = 3; /* Memory write to indicate software region of interest is to pause counting */

stop_pmu();
}
//Resume tracking pmu metrics for region of interest
void resume_marker() {
  SOFTWARE_MARK = 4; /* Memory write to indicate software region of interest is to resume counting*/

start_pmu();
}

// Memory write to indicate the software region of interest is ending
void stop_marker() {
  SOFTWARE_MARK = 0;

stop_pmu();
print_marker();
}

// Print results of counters
void print_marker() {
  printf("\n______________________");
  printf("\n%s:\n\n", PMU_START);

uint32_t cycle_count, inst_count;
  inst_count = ARM_PMU_Get_Chain_EVCNTR(0);
  cycle_count = ARM_PMU_Get_CCNTR();

  printf("%s: %" PRIu32 "\n", PMU_CCNT, cycle_count);
  printf("%s: %" PRIu32 "\n", PMU_EXEC_INSTRS, inst_count);
  printf("%s: %" PRIu32 "\n", PMU_MVE_EXEC_INSTRS, ARM_PMU_Get_Chain_EVCNTR(2));
  printf("%s: %" PRIu32 "\n", PMU_L1_DCACHE_ACCESSES, ARM_PMU_Get_Chain_EVCNTR(4));
  printf("%s: %" PRIu32 "\n", PMU_DTCM_ACCESSES, ARM_PMU_Get_Chain_EVCNTR(6));

  double cycles_per_inst;
  cycles_per_inst = (double)cycle_count / (double)inst_count;
  printf("%s: %lf\n", PMU_AVE_CPI, cycles_per_inst);
// End of printing
  printf("\n%s\n", PMU_END);
  printf("______________________\n");
}

void __reset_cycle_counter() {}

void __start_cycle_counter() {
start = CM_DWT_CYCCNT;
}

void __stop_cycle_counter() {
stop = CM_DWT_CYCCNT;
}

unsigned __get_cycle_count() {
return stop - start;
}
