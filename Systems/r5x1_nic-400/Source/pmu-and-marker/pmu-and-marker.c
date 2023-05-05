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



#include "pmu_ARv7.h"



unsigned int start = 0, stop = 0;


//Start tracking pmu metrics for region of interest
void start_marker() {
  SOFTWARE_MARK = 1; /* Memory write to indicate software region of interest is starting */

enable_pmu_user_access();  // Allow access to PMU from User Mode

  enable_pmu();  // Enable the PMU

  pmn_config(0, 0x8);   // Configure counter 0 to count instructions retired

 pmn_config(1, 0x43);  // Configure counter 1 to count memory accesses
  pmn_config(2, 0x4);   // Configure counter 2 to count L1 dcache accesses
  enable_pmn(2);        // Enable counter 2
 enable_pmn(0);  // Enable counter 0
  enable_pmn(1);  // Enable counter 1
  enable_ccnt();  // Enable CCNT

  reset_ccnt();  // Reset the CCNT (cycle counter)
  reset_pmn();   // Reset the configurable counters
}

// Memory write to indicate the software region of interest is ending
void stop_marker() {
  SOFTWARE_MARK = 0;

disable_ccnt();  // Stop CCNT
  disable_pmn(0);  // Stop counter 0
  disable_pmn(1);  // Stop counter 1
disable_pmn(2);  // Stop counter 2
print_marker();
}

// Print results of counters
void print_marker() {
  printf("\n______________________");
  printf("\n%s:\n\n", PMU_START);

float inst, cycle, cycle_inst;

  inst = (float)read_pmn(0);
  cycle = (float)read_ccnt();
  printf("%s: %u\n", PMU_EXEC_INSTRS, (unsigned int)inst);
  printf("%s: %u\n", PMU_CCNT, (unsigned int)cycle);
  printf("%s: %u\n", PMU_MEM_ACCESSES, read_pmn(1));
  printf("%s: %u\n", PMU_L1_DCACHE_ACCESSES, read_pmn(2));
  cycle_inst = cycle / inst;

  printf("%s: %f\n", PMU_AVE_CPI, cycle_inst);
// End of printing
  printf("\n%s\n", PMU_END);
  printf("______________________\n");
}

void __reset_cycle_counter() {}

void __start_cycle_counter() {
start = read_ccnt();
}

void __stop_cycle_counter() {
stop = read_ccnt();
}

unsigned __get_cycle_count() {
return stop - start;
}
