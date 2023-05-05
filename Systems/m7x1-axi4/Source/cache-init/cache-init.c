/*
** Copyright (c) 2019,2021 Arm Limited. All rights reserved.
*/

#include "serial.h"
#include <stdio.h>

// __builtin_arm_isb is not defined for gcc
#if defined(__GNUC__)
#define __ISB()
#else
#define __ISB() __builtin_arm_isb(0xF)
#endif

#define ICIALLU 0xE000EF50
#define CCR 0xE000ED14 /* configuration and control reg */
void cache_initalize() {
/* Invalidate the caches */
  *(volatile unsigned int*)(ICIALLU) = 0;

  /* Enable both I & D caches */
  *(volatile unsigned int*)(CCR) |= 0xB0000;

  // ISB
  __ISB();
}
