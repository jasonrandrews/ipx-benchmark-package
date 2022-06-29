/**********************************************************************/
/*        Copyright (c) 2020 Arm Limited. All rights reserved.        */  
/*                                                                    */
/*                         IP Selection Sandbox                       */
/**********************************************************************/
/* Linker file for Cortex-M4 with GCC                                 */


STACK_SIZE = 0xa000; /* 4kB of stack memory */

MEMORY
{
  FLASH (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00100000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00100000
}

SECTIONS
{

 . = 0x0;

 .text : 
       {
              *.o (vectors)
              *(.text)
       } > FLASH

 .data : 
       { 
              *(.data)
       } > FLASH

 .ARM.exidx : 
       { 
              __exidx_start = .;
             *(.ARM.exidx* .gnu.linkonce.armexidx.*) 
              __exidx_end = .;
       } > FLASH

 .bss :
       {
              . = ALIGN(4);
              __bss_start__ = .;
              *(.bss*)
              *(COMMON)
              . = ALIGN(4);
              __bss_end__ = .;
       } > FLASH

 .heap (COPY):
       {
              __end__     = .;     /* not neccecary? */
              __HeapBase  = .;     /* not neccecary? */
              PROVIDE(end = .);    /* used in v6-m/nofp/libnosys.a -> _sbrk */
              *(.heap*)
              __HeapLimit = .;     /* not neccecary? */
       } > FLASH

 . = ALIGN(8);

__StackLimit = .;                  /* not neccecary? */
 . = . + STACK_SIZE; 
 stack_top  = .;                   /* used in startup code to set vectors to SP */
 __StackTop = .;                   /* not neccecary? */
 PROVIDE(__stack = .);             /* not neccecary? */

}
