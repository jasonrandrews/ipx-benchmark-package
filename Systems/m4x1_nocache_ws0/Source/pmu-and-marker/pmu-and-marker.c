/*********************************************************************/
/*        Copyright (c) 2020-2021 Arm Limited. All rights reserved.  */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/
/* PMU functionality for all systems */

#include "ReportMessages/messages.h"
#include "pmu-and-marker.h"
#include "serial.h"
#include <stdio.h>

#include "pmu_M.h"





unsigned int start = 0, stop = 0;


//Start tracking pmu metrics for region of interest
void start_marker() {
  SOFTWARE_MARK = 1; /* Memory write to indicate software region of interest is starting */

start_systick();  // Enable and start systick timer
}

// Memory write to indicate the software region of interest is ending
void stop_marker() {
  SOFTWARE_MARK = 0;

stop_systick();

  print_marker();
}

// Print results of counters
void print_marker() {
  printf("\n______________________");
  printf("\n%s:\n\n", PMU_START);

printf("%s: %u\n", PMU_CCNT, (SysTick_START - SysTick_CVR));
  if (SysTick_CSR & 0x10000)
    printf("WARNING: counter has overflowed, more than 16,777,215 cycles");
// End of printing
  printf("\n%s\n", PMU_END);
  printf("______________________\n");
}

void __reset_cycle_counter() {}

void __start_cycle_counter() {
start = SysTick_CVR;
}

void __stop_cycle_counter() {
stop = SysTick_CVR;
}

unsigned __get_cycle_count() {
return start - stop;
}
