/*
** Copyright (c) 2020-2021 Arm Limited. All rights reserved.
*/

#include "serial.h"
#include <stdio.h>

void fpu_initalize() {
  // Enable the FPU in both privileged and user modes by setting bits 20-23 of CPACR to enable CP10 and CP11
  __asm("LDR R0, =0xE000ED88");
  __asm("LDR R1, [R0]");
  __asm("ORR R1, R1, #(0xF << 20)");
  __asm("STR R1, [R0]");
  __asm("ISB");
  __asm("DSB");
}
