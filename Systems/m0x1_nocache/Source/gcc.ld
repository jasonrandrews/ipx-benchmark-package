/**********************************************************************/
/*        Copyright (c) 2020 Arm Limited. All rights reserved.        */  
/*                                                                    */
/*                         IP Selection Sandbox                       */
/**********************************************************************/
/* Linker file for GCC                                                */


/*************************** MEMORY ***********************************/
/* name (attr) : ORIGIN = start-addr, LENGTH = mem-size               */

/* name: defines region names, passed to Linker                       */

/* attribute (attr) options:                                          */
/*     Letter |    Section Attribute                                  */
/*     _______________________________________________________        */
/*     r      |    Read-only sections                                 */
/*     w      |    Read/write sections                                */
/*     x      |    Sections containing executable code                */
/*     i      |    Initalized sections                                */
/*     l      |    Same as 'i'                                        */
/*     !      |    Invert any of the following attributes             */

/* ORIGIN: specifies the start address of the memory region           */
/*            in the physical memory                                  */

/* LENGTH: specifies the size of the physical memory region           */

/* example ->                                                         */
/*     RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00a00000           */
/*            Memory starting at 0x0 being 10Mb in size that          */
/*            has read, write, and executable code (named RAM)        */            
/**********************************************************************/



/************************** SYMBOLS ***********************************/
/* Symbol Name       |      Usage                                     */
/* __________________|_______________________________________________ */
/* .text             | Code, vector table, and constants              */
/* .data             | Initalized variables                           */
/* .ARM.exidx        | Variables needed for callstack unwinding       */
/* __exidx_start     | C lib: Unwinds during exception handling phase */
/* .ARM.exidx*       | Variables needed for callstack unwinding       */
/* .gnu.linkonce.armexidx.* | Needed for callstack unwinding          */
/* __exidx_end       | C lib: Unwinds during exception handling phase */
/* .bss              | Uninitalized data (initalized as 0 in startup) */
/*  __bss_start__    | Used by C library                              */
/*  __bss_end__      | Used by C library                              */
/* __end__           |                                                */
/* .heap             |                                                */
/* __HeapBase        | ?? is this used                                */
/* __HeapLimit       |                                                */
/* .stack            | Must explicitly reserve space for it           */
/* .stack_dummy      | Linker gets size of stack sections             */
/* __StackLimit      | Boot stack 'ends' here                         */
/* __StackTop        | Boot stack 'starts' and grows down from here   */
/* __stack           | same as __StackTop                             */
/*                   |                                                */
/* vectors           |            vector table contents               */ 
/**/
/**********************************************************************/

/* OTHER NOTES                                                        */
/* PROVIDE keyword may be used to define a symbol, such as `etext',   */ 
/*     only if it is referenced but not defined.                      */





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
