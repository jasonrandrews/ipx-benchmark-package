/*---------------------------------------------------------------*/
/* Template for user defined benchmarks 					*/
/*                                                               */
/* Copyright (C) ARM Limited, 2019. All rights reserved.    */
/*---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

extern void start_marker();
extern void stop_marker();

int main() {
  (void)start_marker();

  // Insert user defined code here
  printf("app-template code goes here.\n");
  // End user defined code here

  (void)stop_marker();

  return 0;
}
