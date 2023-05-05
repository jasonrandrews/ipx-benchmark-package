/*********************************************************************/
/*      Copyright (c) 2020,2022 Arm Limited. All rights reserved.    */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/
/* PMU functions for Cortex-M Cores */

#include "pmu_M.h"
#include <stdio.h>

/* DWT settings, for Cortex-M cores that use DWT */
void init_dwt() {
  CM_DEMCR |= CM_TRCENA_BIT;
  CM_DWT_LAR = 0xC5ACCE55;
  CM_DWT_CYCCNT = 0;
}
  void start_dwt() {
  CM_DWT_CONTROL = CM_DWT_CYCCNTENA_BIT | CM_DWT_CPIEVTENA_BIT | CM_DWT_EXCEVTENA_BIT | CM_DWT_SLEEPEVTENA_BIT | CM_DWT_LSUEVTENA_BIT | CM_DWT_FOLDEVTENA_BIT;
}
void stop_dwt() {
  CM_DWT_CONTROL = ~CM_DWT_CYCCNTENA_BIT & ~CM_DWT_CPIEVTENA_BIT & ~CM_DWT_EXCEVTENA_BIT & ~CM_DWT_SLEEPEVTENA_BIT & ~CM_DWT_LSUEVTENA_BIT & ~CM_DWT_FOLDEVTENA_BIT;
}

/* systick settings, for Cortex-M cores that use systick */
void start_systick() {
  SysTick_RVR = SysTick_START;
  SysTick_CVR = 0;
  SysTick_CSR |= (SysTick_Enable | SysTick_ClockSource);
}
void stop_systick() {
  SysTick_CSR &= ~SysTick_Enable;
}

// Declarations for PMU Helper functions for V8.1-M CPUs
__STATIC_INLINE void ARM_PMU_Enable(void);
__STATIC_INLINE void ARM_PMU_Set_EVTYPER(uint32_t num, uint32_t type);
__STATIC_INLINE void ARM_PMU_CYCCNT_Reset(void);
__STATIC_INLINE void ARM_PMU_EVCNTR_ALL_Reset(void);

// TODO: Figure out why the static inline didn't work. Do we need to
// say extern inline
// Initialize the PMU. For V8.1M CPUs only
void init_pmu() {
  // Turn on the PMU
  CM_DEMCR |= CM_TRCENA_BIT;
  ARM_PMU_Enable();

  // Enable the counters we want
  // For the odd counters, we use the CHAIN event to extend the size of the
  // even counters from 16 bits to 32 bits
  ARM_PMU_Set_EVTYPER(0, ARM_PMU_INST_RETIRED);
  ARM_PMU_Set_EVTYPER(1, ARM_PMU_CHAIN);
  ARM_PMU_Set_EVTYPER(2, ARM_PMU_MVE_INST_RETIRED);
  ARM_PMU_Set_EVTYPER(3, ARM_PMU_CHAIN);
  ARM_PMU_Set_EVTYPER(4, ARM_PMU_L1D_CACHE);
  ARM_PMU_Set_EVTYPER(5, ARM_PMU_CHAIN);
  ARM_PMU_Set_EVTYPER(6, ARM_PMU_DTCM_ACCESS);
  ARM_PMU_Set_EVTYPER(7, ARM_PMU_CHAIN);

  // Reset the counters down to zero
  ARM_PMU_EVCNTR_ALL_Reset();
  ARM_PMU_CYCCNT_Reset();
}

// Enable the PMU
__STATIC_INLINE void ARM_PMU_Enable(void) {
  PMU->CTRL |= PMU_CTRL_ENABLE_Msk;
}

// Set event to count for PMU eventer counter
__STATIC_INLINE void ARM_PMU_Set_EVTYPER(uint32_t num, uint32_t type) {
  PMU->EVTYPER[num] = type;
}

// Reset cycle counter
__STATIC_INLINE void ARM_PMU_CYCCNT_Reset(void) {
  PMU->CTRL |= PMU_CTRL_CYCCNT_RESET_Msk;
}

// Reset all event counters
__STATIC_INLINE void ARM_PMU_EVCNTR_ALL_Reset(void) {
  PMU->CTRL |= PMU_CTRL_EVENTCNT_RESET_Msk;
}
