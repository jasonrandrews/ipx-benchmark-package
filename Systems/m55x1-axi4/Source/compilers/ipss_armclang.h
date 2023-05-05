/*********************************************************************/
/*        Copyright (c) 2022 Arm Limited. All rights reserved.       */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/

#ifndef __IPSS_ARMCLANG_H__
#define __IPSS_ARMCLANG_H__

// Compiler specific defines
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE       static __inline
#endif

// Compiler specific main function call
extern int __main(void);
#ifndef __PROGRAM_START
#define __PROGRAM_START         __main
#endif

#endif // __IPSS_ARMCLANG_H__
