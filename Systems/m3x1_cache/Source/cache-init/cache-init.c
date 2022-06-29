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

void cache_initalize() {
}
