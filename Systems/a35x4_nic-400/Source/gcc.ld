/**********************************************************************/
/*        Copyright (c) 2020 Arm Limited. All rights reserved.        */  
/*                                                                    */
/*                         IP Selection Sandbox                       */
/**********************************************************************/
/* Linker file for Cortex-A35 with GCC                                */


STACK_SIZE = 0x80000; /* ??kB of stack memory */


MEMORY
{
  FLASH (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
  /* RAM (rwx) : ORIGIN = 0x00040000, LENGTH = 0x00100000 */    /* length: 1Mb */ 
  CS3_PERIPHERALS (rwx) : ORIGIN = 0x13080000, LENGTH =  0x10000  /* copied directly from scatter */
}                      



SECTIONS
{

 . = 0x0;

 .code : 
       {
              __code_base = .;
              *.o (StartUp)
              *(.text)
              *.o (PMU)             /* need explict mention or it overlaps with vectors at address 0 */
              __code_limit = .;
       } > FLASH

 .data : 
       { 
              __data_base = .;
              *(.data)
              __data_limit = .;
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


/* Stacks for EL3 */
/******************/
 . = ALIGN(8);
__EL3_stack_limit = .;            
 . = . + 0x1000; 
 __EL3_stack_top  = .;                 


/* L1 Tables */
/******************/
 . = ALIGN(4096);
__TTB0_L1_start = .;            
 . = . + 0x1000; 
 __TTB0_L1_end  = .;       


/* L2 Tables */
/* note: alignment is 4kb (4096 in scatter) */
/******************/
 . = ALIGN(4096);
__TTB0_L2_RAM_start = .;            
 . = . + 0x1000; 
 __TTB0_L2_RAM_end  = .;   

 . = ALIGN(4096);
__TTB0_L2_PRIVATE_start = .;            
 . = . + 0x1000; 
 __TTB0_L2_PRIVATE_end  = .;   


/* Startup code use to calculate top of RAM memory*/
/******************/
 . = ALIGN(4);
__RAM_top = .;            



/* Peripheral callout*/
/******************/
 .cs3_peripherals : 
       {
              __CS3_peripherals_start = .;
       } > CS3_PERIPHERALS

}
