/*
** Copyright (c) 2020-2021 Arm Limited. All rights reserved.
*/

#include "serial.h"
#include <stdio.h>

void fpu_initialize() {
  __asm("MRC P15, 0, R0, C1, C0, 2");  // Read Coprocessor Access Control Register (CPACR)
  __asm("ORR R0, R0, #(0xF << 20)");   // Enable access to CP 10 & 11
  __asm("MCR P15, 0, R0, C1, C0, 2");  // Write Coprocessor Access Control Register (CPACR)
  __asm("ISB");
  __asm("DSB");
  __asm("MOV R0, #0x40000000");  // Switch on VFP hardware
  __asm("VMSR FPEXC, R0");       // Write FPEXC register, EN bit set
}
