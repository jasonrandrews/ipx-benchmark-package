/*********************************************************************/
/*        Copyright (c) 2022 Arm Limited. All rights reserved.       */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/

#ifndef __IPSS_GCC_H__
#define __IPSS_GCC_H__

// Compiler specific defines
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE       static inline
#endif

// Compiler specific main function call
#ifndef __PROGRAM_START
/**
  Initialize the data regions for GCC and call _start()
 */
#include <stdint.h>
__attribute__((always_inline)) static inline void _ipss_gcc_start(void) {
  extern int _start(void);

  // These variables are defined in the gcc.ld linker file
  extern uint32_t __etext;
  extern uint32_t __data_start__;
  extern uint32_t __data_end__;

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

  // Call the main function of the program
  _start();
}

#define __PROGRAM_START         _ipss_gcc_start
#endif

#endif // __IPSS_GCC_H__
