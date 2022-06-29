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





#include "pmu_Av8.h"

unsigned int start = 0, stop = 0;


//Start tracking pmu metrics for region of interest
void start_marker() {
  SOFTWARE_MARK = 1; /* Memory write to indicate software region of interest is starting */

enablePMU();  // Enable PMU

  pmnConfig(0, 0x8, 0);  // Configure counter 0 to count intructions retired
  pmnConfig(1, 0x1, 0);  // Configure counter 1 to count ICache Miss
  pmnConfig(2, 0x4, 0);  // Configure counter 2 to count DCache Miss
  pmnConfig(3, 0x6, 0);  // Configure counter 3 to count Memory Reads

  // Enable the Programmable Counters
  enablePMN(0);
  enablePMN(1);
  enablePMN(2);
  enablePMN(3);
  enableCCNT();

  // Reset CCNT and the Counters
  resetCCNT();
  resetPMN();
}

// Memory write to indicate the software region of interest is ending
void stop_marker() {
  SOFTWARE_MARK = 0;

disableCCNT();
  disablePMN(0);
  disablePMN(1);
  disablePMN(2);
  disablePMN(3);

  print_marker();
}

// Print results of counters
void print_marker() {
  printf("\n______________________");
  printf("\n%s:\n\n", PMU_START);

float inst, cycl, cycl_inst;

  printf("%s: %u\n", PMU_EXEC_INSTRS, readPMN(0));
  printf("%s: %llu\n", PMU_CCNT, readCCNT());
  printf("%s: %u\n", PMU_ICACHE_MISSES, readPMN(1));
  printf("%s: %u\n", PMU_L1_DCACHE_MISSES, readPMN(2));
  printf("%s: %u\n", PMU_MEM_READS, readPMN(3));

  inst = readPMN(0);
  cycl = readCCNT();
  cycl_inst = cycl / inst;

  printf("%s: %f\n", PMU_AVE_CPI, cycl_inst);
// End of printing
  printf("\n%s\n", PMU_END);
  printf("______________________\n");
}

void __reset_cycle_counter() {}

void __start_cycle_counter() {
start = readCCNT();
}

void __stop_cycle_counter() {
stop = readCCNT();
}

unsigned __get_cycle_count() {
return stop - start;
}
