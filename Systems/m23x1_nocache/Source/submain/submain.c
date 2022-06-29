/*
** Copyright (c) 2019 Arm Limited. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>

#include "serial.h"      // for record_boot
#include "cache-init.h"  // for cache_initalize
#include "fpu-init.h"    // for fpu_initalize

extern int __main(void);
extern int _start(void);

void record_boot(void);

void submain(void) {
  (void)fpu_initalize();  // Initalize FPU (floating point unit) for cores that need it

  (void)cache_initalize();  // Initalize caches right away. Implmentation varies by core

  (void)record_boot();  // Record (by printing to stdout) the cycles to this point, the boot code time

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)  // AC6
  __main();
#elif defined(__GNUC__)  // GCC
  _start();
#endif
}

void record_boot(void) {
  SOFTWARE_MARK = 2;  // write to indicate startup code is complete
}
