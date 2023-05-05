/**********************************************************************/
/*     Copyright (c) 2020,2022 Arm Limited. All rights reserved.      */
/*                                                                    */
/*                         IP Selection Sandbox                       */
/**********************************************************************/
/* Linker file for Cortex-M7, M55, M85 with GCC                       */


/*--------------------- Stack / Heap Configuration ----------------------------
  Stack Size (in Bytes) 0x0-0xFFFFFFFF
  Heap Size (in Bytes) 0x0-0xFFFFFFFF
-----------------------------------------------------------------------------*/
__STACK_SIZE = 0x0000a000;
__HEAP_SIZE  = 0x00100000;

/*
Set the regions of memory size for CODE and DATA memory
Limit DATA size to 16MB to match that of the largest possible DTCM

For additional information about this system and the the size of the
memories in it, see the scatter.scat file in the same directory as this
one. Both files map memory the same way for their respective compiler.
*/
MEMORY
{
  CODE (rx) : ORIGIN = 0x00000000, LENGTH = 0x10000000
  DATA (rwx) : ORIGIN = 0x20000000, LENGTH = 0x01000000
}

SECTIONS
{
  /* Send the text and RO data to the CODE memory */
  .text :
  {
    KEEP(*.o (vectors))
    *(.text)

    /*
    Both of these sections must be explicitly marked as KEEP otherwise they will be removed
    if using --gc-sections
    Thes hold code that runs before and after main is called
    */
    KEEP(*(.init))
    KEEP(*(.fini))
  } > CODE

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > CODE

  /*
  The following code is adapted from the CMSIS M series linker examples
  Sends the data and bss sections to RAM/DATA and sets up the stack and heap
  */
  /*
   * Location counter can end up 2byte aligned with narrow Thumb code but
   * __etext is assumed by startup code to be the LMA of a section in RAM
   * which must be 4byte aligned.
   */
  __etext = ALIGN (4); /* Align and mark the region right before data starts as the end of the text */

  .data : AT (__etext)
  {
    __data_start__ = .;
    *(vtable)
    *(.rodata)
    *(.rodata.*)
    *(.data)
    *(.data.*)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    KEEP(*(.jcr*))
    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;

  } > DATA

 .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > DATA AT > DATA

  /* Heap grows up from this point */
  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > DATA

  /*
    Stack grows down from the top of DATA
    For best performance, we set the stack to start at the top of the default DTCM size.
    If desired, you may move the stack out of DTCM and into RAM.
  */
  .stack (ORIGIN(DATA) + LENGTH(DATA) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    stack_top  = .;                   /* used in startup code to set vectors to SP */
    __StackTop = .;
  } > DATA
  PROVIDE(__stack = __StackTop);

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "Stack and Heap regions collided")
}
