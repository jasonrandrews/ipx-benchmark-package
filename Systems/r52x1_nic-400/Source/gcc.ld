/**********************************************************************/
/*        Copyright (c) 2020 Arm Limited. All rights reserved.        */  
/*                                                                    */
/*                         IP Selection Sandbox                       */
/**********************************************************************/
/* Linker file for Cortex-R52 with GCC                                */


STACK_SIZE = 0x10000; /* 64kB of stack memory */


MEMORY
{
  FLASH (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
  RAM (rwx) : ORIGIN = 0x00040000, LENGTH = 0x00100000    /* length: 1Mb */
}                      



SECTIONS
{

 . = 0x0;

 .code : 
       {
              __code_base = .;
              *.o (vectors)
              *(.text)
              *.o (PMU)             /* need explict mention or it overlaps with vectors at address 0 */
              __code_limit = .;
       } > FLASH

 .data : 
       { 
              __data_base = .;
              *(.data)
              __data_limit = .;
       } > RAM

 .ARM.exidx : 
       { 
              __exidx_start = .;
             *(.ARM.exidx* .gnu.linkonce.armexidx.*) 
              __exidx_end = .;
       } > RAM

 .bss :
       {
              . = ALIGN(4);
              __bss_start__ = .;
              *(.bss*)
              *(COMMON)
              . = ALIGN(4);
              __bss_end__ = .;
       } > RAM

 .heap (COPY):
       {
              __end__     = .;     /* not neccecary? */
              __HeapBase  = .;     /* not neccecary? */
              PROVIDE(end = .);    /* used in v6-m/nofp/libnosys.a -> _sbrk */
              *(.heap*)
              __HeapLimit = .;     /* not neccecary? */
       } > RAM

 . = ALIGN(8);

__StackLimit = .;                  /* not neccecary? */
 . = . + STACK_SIZE; 
 stack_top  = .;                   /* used in startup code to set vectors to SP */
 __StackTop = .;                   /* not neccecary? */
 PROVIDE(__stack = .);             /* not neccecary? */

}
