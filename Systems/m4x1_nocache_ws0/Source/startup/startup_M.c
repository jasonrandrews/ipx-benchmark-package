/*
** Copyright (c) 2020 Arm Limited. All rights reserved.
*/

// Initial Setup
//----------------------------------------------------------------

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)  // AC6
extern unsigned int
    Image$$ARM_LIB_STACKHEAP$$ZI$$Limit;  // Access top of stackheap for combined stack/heap one region area
#define STACK_TOP &Image$$ARM_LIB_STACKHEAP$$ZI$$Limit  // Define unified variable between compilers

typedef void (*const ExecFuncPtr)(void)
    __attribute__((interrupt));  // typedef for the function pointers in the vector table
extern int __main(void);         // init C library from this compiler

#elif defined(__GNUC__)       // GCC
extern unsigned int stack_top;  // Access top of stack as defined in the linker file
#define STACK_TOP &stack_top  // Define unified variable between compilers

typedef void (*const __attribute__((interrupt))
              ExecFuncPtr)(void);  // typedef for the function pointers in the vector table
extern int _start(void);           // init C library from this compiler
#endif

extern int submain();  // Refer to submain function, same for both compilers
//----------------------------------------------------------------

// Exception Vector Table & Handlers
//----------------------------------------------------------------
__attribute__((interrupt)) void ResetHandler(void) {
  submain();
}

__attribute__((interrupt)) void NMIException(void) {
  while (1)
    ;
}
__attribute__((interrupt)) void HardFaultException(void) {
  while (1)
    ;
}
__attribute__((interrupt)) void SVCHandler(void) {
  while (1)
    ;
}
__attribute__((interrupt)) void PendSVC(void) {
  while (1)
    ;
}
__attribute__((interrupt)) void InterruptHandler(void) {
  while (1)
    ;
}
extern __attribute__((interrupt)) void SysTickHandler(void);  // defined in timer.c

ExecFuncPtr vector_table[] __attribute__((section("vectors"))) = {
    (ExecFuncPtr)STACK_TOP,  // initial SP
    ResetHandler,            // initial PC/Reset
    NMIException,            // NMI
    HardFaultException,      // Hard Fault
    0,                       // Memory Manage Fault
    0,                       // Bus Fault
    0,                       // Usage Fault
    0,                       // RESERVED
    0,                       // RESERVED
    0,                       // RESERVED
    0,                       // RESERVED
    SVCHandler,              // SVCHandler
    0,                       // RESERVED for debug
    0,                       // RESERVED
    PendSVC,                 // Pending SVC
    InterruptHandler,        // generic...
    InterruptHandler,        // generic...
    InterruptHandler,        // generic...
    InterruptHandler,        // generic...
    InterruptHandler         // generic...
};
