//-----------------------------------------------------------------------------
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from ARM Limited or its affiliates.
//
//            (C) COPYRIGHT 2014-2015 ARM Limited or its affiliates.
//                ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from ARM Limited or its affiliates.
//
//      SVN Information
//
//      Checked In          : $Date: 2015-09-25 17:40:00 +0100 (Fri, 25 Sep 2015) $
//
//      Revision            : $Revision: 320901 $
//
//      Release Information : Kite Processor MP040-r0p0-00bet0
//
//------------------------------------------------------------------------------

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include "serial.h"
//#include <output_tube.h>

extern unsigned int PMN_Reset(void);
extern unsigned int PMN_Stop(void);
extern unsigned int PMN_Read_Overflow(void);
extern unsigned int PMN_Read_Cycles(void);
extern unsigned int PMN_Read_Reg(int);
extern const char* PMN_Metric(int);

volatile int iter_count;
volatile unsigned int iter_table[ITERATIONS];

#define MAINSTART
#define MAINEND return 0;

#ifdef ACTIVATE_PMU
#define BENCHSTART \
  TCIterInit();    \
  TCStartTimer();
#define BENCHFINISHED \
  TCStopTimer();      \
  benchmark_completed_success();
#define LOOPSTART
#define LOOPEND TCIterTimer();
#else
#define BENCHSTART start_marker();
//#define BENCHFINISHED   benchmark_completed_success();
#define BENCHFINISHED stop_marker();
#define LOOPSTART
#define LOOPEND
#endif

#ifdef ACTIVATE_PMU
// This functions initialize the overall counter and the counter per iteration
static void TCIterInit() {
  int i;
  for (i = 0; i < ITERATIONS; i++)
    iter_table[i] = 0;
  iter_count = 0;
}

// This functions resets and starts the counters
static void TCStartTimer() {
  PMN_Reset();
}

// This functions saves the result of one iteration
// NOTE: as this operation takes some cycles, saving the results every iterations affects the results
static void TCIterTimer() {
  iter_table[iter_count] = PMN_Read_Cycles();
  iter_count++;
}

// This function stops the timer and prints the results
static void TCStopTimer() {
  unsigned int cc, ov, i, val;
  const char* metric;
  char message[100];
  const unsigned int max_pmn = 6;
  double cycles_per_iter, instr_per_iter, iterations;

  PMN_Stop();

  ov = PMN_Read_Overflow();

  cc = PMN_Read_Cycles();
  // printf( "Perf monitor:     Cycles = %llu%s\n", (long long)cc, (ov & 0x80000000) ? " (Overflow)" : "");
  print_string("Perf monitor:     Cycles = ");
  print_int(cc);
  print_string((ov & 0x80000000) ? " (Overflow)\n" : "\n");

  for (i = 0; i < max_pmn; i++) {
    metric = PMN_Metric(i);
    if (metric[0] != '\0') {
      val = PMN_Read_Reg(i);
      // printf( "Perf metric #%d:  %s = %u%s\n", i, metric, val, (ov & (1 << i)) ? " (Overflow)" : "");
      print_string("Perf metric #");
      print_int(i);
      print_string(":   ");
      print_string((char*)metric);
      print_string(" = ");
      print_int(val);
      print_string((ov & 0x80000000) ? " (Overflow)\n" : "\n");
    }
  }

  // printf( "Perf overflow:    %08x\nIterations time:\n", ov);
  print_string("Perf overflow:    ");
  print_int(ov);
  print_string("\n\nIterations time:\n");
  for (i = 0; i < ITERATIONS; i++) {
    // printf( "Iter %-2d: %u\n", i+1, i == 0 ? iter_table[0] : iter_table[i]-iter_table[i-1]);
    print_string("  Iter ");
    print_int(i + 1);
    print_string(": ");
    print_int((i == 0) ? iter_table[0] : iter_table[i] - iter_table[i - 1]);
    print_string("\n");
  }
  // printf( "\nBuilt on %s at %s\n\n", __DATE__, __TIME__);
  print_string("\nBuilt on ");
  print_string(__DATE__);
  print_string(" at ");
  print_string(__TIME__);
  print_string("\n\n");
}
#endif
#endif
