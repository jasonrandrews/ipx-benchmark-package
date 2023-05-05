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
init_dwt();
/* Memory write to indicate software region of interest is starting
     This should be written after PMU/counters have been initialized
     but before the counters have been enabled
  */
  SOFTWARE_MARK = 1;

start_dwt();  // Enable and start data watchdog timers
}

//Pause tracking pmu metrics for region of interest
void pause_marker() {
  SOFTWARE_MARK = 3; /* Memory write to indicate software region of interest is to pause counting */

stop_dwt();
}
//Resume tracking pmu metrics for region of interest
void resume_marker() {
  SOFTWARE_MARK = 4; /* Memory write to indicate software region of interest is to resume counting*/

start_dwt();
}

// Memory write to indicate the software region of interest is ending
void stop_marker() {
  SOFTWARE_MARK = 0;

stop_dwt();
print_marker();
}

// Print results of counters
void print_marker() {
  printf("\n______________________");
  printf("\n%s:\n\n", PMU_START);

printf("%s: %" PRIu32 "\n", PMU_CCNT, CM_DWT_CYCCNT);
  printf("%s: %" PRIu32 "\n", PMU_CPICNT, CM_DWT_CPICNT);
  printf("%s: %" PRIu32 "\n", PMU_EXCCNT, CM_DWT_EXCCNT);
  printf("%s: %" PRIu32 "\n", PMU_SLEEPCNT, CM_DWT_SLEEPCNT);
  printf("%s: %" PRIu32 "\n", PMU_LSUCNT, CM_DWT_LSUCNT);
  printf("%s: %" PRIu32 "\n", PMU_FOLDCNT, CM_DWT_FOLDCNT);
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
