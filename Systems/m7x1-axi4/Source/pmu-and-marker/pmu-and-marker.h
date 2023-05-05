/*********************************************************************/
/*      Copyright (c) 2020-2022 Arm Limited. All rights reserved.    */
/*                                                                   */
/*                         IP Selection Sandbox                      */
/*********************************************************************/
/* General PMU functions */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
void start_marker(void);
void stop_marker(void);
void resume_marker(void);
void pause_marker(void);
void print_marker(void);

void __reset_cycle_counter();
void __start_cycle_counter();
void __stop_cycle_counter();
unsigned __get_cycle_count();
#ifdef __cplusplus
}
#endif
