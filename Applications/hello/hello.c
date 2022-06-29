/*---------------------------------------------------------------*/
/* Performance Monitoring Unit (PMU) Example Code for Cortex-A/R */
/*                                                               */
/* Copyright (C) ARM Limited, 2010-2012. All rights reserved.    */
/*---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

extern void start_marker();
extern void stop_marker();

int main() {
  (void)start_marker();

  printf("Hello world!\n");

  (void)stop_marker();

  return 0;
}
