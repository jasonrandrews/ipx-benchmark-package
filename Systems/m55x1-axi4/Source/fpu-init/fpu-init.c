/*
** Copyright (c) 2022-2023 by Arm Limited (or its affiliates). All Rights Reserved.
*/

#include "serial.h"
#include <stdio.h>

void fpu_initalize() {
// Don't enable the FPU if we pass in the below flag
// This happens when we turn off FP instructions in the compiler
#ifndef NO_FPU
// Enable the FPU registers in both privileged and user modes by setting bits 20-23 of CPACR to enable CP10 and CP11
  // Note that for M55 and M85 (and other v8.1-M CPUs) CP10 also controls access to the MVE registers
  // because they are aliases of the FPU registers
  __asm("LDR R0, =0xE000ED88");
  __asm("LDR R1, [R0]");
  __asm("ORR R1, R1, #(0xF << 20)");
  __asm("STR R1, [R0]");
  __asm("ISB");
  __asm("DSB");
#endif
}
