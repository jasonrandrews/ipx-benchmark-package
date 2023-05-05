/*
** Copyright (c) 2019,2022 Arm Limited. All rights reserved.
*/

// Choose the correct header for the compiler
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#include "ipss_armclang.h"
#elif defined(__GNUC__)
#include "ipss_gcc.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "serial.h"      // for record_boot
#include "cache-init.h"  // for cache_initalize
#include "fpu-init.h"    // for fpu_initalize
#include "pl011_uart.h"  // for PL011 initialization

void record_boot(void);

void submain(void) {
  (void)pl011_uart_init(SERIAL_DATA); // Initialize the PL011 UART

  (void)fpu_initalize();  // Initalize FPU (floating point unit) for cores that need it

  (void)cache_initalize();  // Initalize caches right away. Implmentation varies by core

  (void)record_boot();  // Record (by printing to stdout) the cycles to this point, the boot code time

  // Call the main function of the program
  __PROGRAM_START();
}

void record_boot(void) {
  SOFTWARE_MARK = 2;  // write to indicate startup code is complete
}
