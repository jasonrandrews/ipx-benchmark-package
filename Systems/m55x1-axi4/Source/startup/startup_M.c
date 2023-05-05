/*
** Copyright (c) 2020, 2022-2023 Arm Limited. All rights reserved.
*/

#include <stdio.h>  // For printf in the exception handlers

//----------------------------------------------------------------
// Initial Setup
//----------------------------------------------------------------
typedef void (*const ExecFuncPtr)(void);  // typedef for the function pointers in the vector table

// Define unified variable for the top of the stack between compilers
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)  // AC6
#define STACK_TOP Image$$ARM_LIB_STACK$$ZI$$Limit

#elif defined(__GNUC__)       // GCC
#define STACK_TOP stack_top

#endif

extern unsigned int STACK_TOP;

extern void simulation_exit(void);

//----------------------------------------------------------------
// Exception Vector Table & Handlers
//----------------------------------------------------------------
void ResetHandler       (void);
void Default_Handler    (void);
void NMI_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler  (void) __attribute__ ((weak));
void SVC_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void InterruptHandler  (void) __attribute__ ((weak, alias("Default_Handler")));

extern int submain();  // Refer to submain function, same for both compilers
void ResetHandler(void) {
  submain();
}

// Here we define two exception handlers that can be overridden or modified as desired
// For now we exit simulation if there is fault
void Default_Handler(void) {
  printf("An exception occured. Exiting Simulation.\n");
  simulation_exit();
}

void HardFault_Handler(void) {
  printf("A hard fault exception occured. Exiting Simulation.\n");
  simulation_exit();
}

// This table is not populated with handlers for all faults to keep
// compatibility between all M series cores. As the exception handlers are
// minimally implemented anyways, this shouldn't be a problem unless we have
// an error in execution and the fault is raised.
ExecFuncPtr vector_table[] __attribute__((used, section("vectors"))) = {
  (ExecFuncPtr)(&STACK_TOP),  // initial SP
  ResetHandler,            // initial PC/Reset
  NMI_Handler,             // NMI
  HardFault_Handler,       // Hard Fault
  0,                       // Memory Manage Fault
  0,                       // Bus Fault
  0,                       // Usage Fault
  0,                       // RESERVED
  0,                       // RESERVED
  0,                       // RESERVED
  0,                       // RESERVED
  SVC_Handler,             // SVCHandler
  0,                       // RESERVED for debug
  0,                       // RESERVED
  PendSV_Handler,          // Pending SVC
  InterruptHandler,        // generic...
  InterruptHandler,        // generic...
  InterruptHandler,        // generic...
  InterruptHandler,        // generic...
  InterruptHandler         // generic...
};
