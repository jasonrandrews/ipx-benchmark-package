/*
** Copyright (c) 2019, 2022 Arm Limited. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>

#include "serial.h"      // for record_boot
#include "cache-init.h"  // for cache_initalize
#include "fpu-init.h"    // for fpu_initalize

extern int __main(void);
extern int _start(void);
void record_boot(void);

#if !(defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#include <stdint.h>
// These variables are defined in the gcc.ld linker file
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
#endif


void submain(void) {
  (void)fpu_initialize();  // Initialize FPU (floating point unit) for cores that need it

  (void)cache_initialize();  // Initialize caches right away. Implementation varies by core

  (void)record_boot();  // Record (by printing to stdout) the cycles to this point, the boot code time

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)  // AC6

  // AC6 automatically copies data from the load to execution region after caches have been
  // initialized. So nothing extra to add here.
__main();
#elif defined(__GNUC__)  // GCC

  // GCC does not automatically copy the .data section from the load region
  // to the execution region. The below code does it for us.
  // We want to do this after caches have been initialized so we get the benefit of using them
  // Note that GCC does initialize the .bss data automatically
  // You can verify this by turning off caches and looking at the AXI logger log for the DATA RAM

  // This was adapted from the cmsis_gcc.h file in the Arm CMSIS codebase
  // See also the binutils manual("Output Section LMA") for a description of the LMA and VMA

  // __etext is the last memory address before the .data section in the scatter file
  // AKA it is the start of the load region (LMA) for the .data section
  uint32_t* load_address = &__etext;

  // __data_start__ is the first memory address inside of the .data section
  // AKA it is the start of the execution region (VMA) for the .data section
  uint32_t* virtual_address = &__data_start__;

  // Make sure the LMA and VMA are different
  // If so, copy the data from the LMA to the VMA, one pointer at a time
  if (load_address != virtual_address) {
    for (; virtual_address < &__data_end__;) {
      *virtual_address++ = *load_address++;
    }
  }

  _start();
#endif
}

void record_boot(void) {
  SOFTWARE_MARK = 2;  // write to indicate startup code is complete
}