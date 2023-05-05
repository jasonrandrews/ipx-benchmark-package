/*
** Copyright (c) 2019,2021-2022 Arm Limited. All rights reserved.
*/

#ifndef __SERIAL_H
#define __SERIAL_H

#define SERIAL_BASE 0xa0000000
// Use pl011_uart_t for the type of the SERIAL_DATA pointer
typedef struct pl011_uart_t pl011_uart_t;

#define SERIAL_DATA ((pl011_uart_t *)SERIAL_BASE)

#define EXECUTION_TRACKER_BASE 0xa0001000

// Use software_marker_t for the type of the SOFTWARE_MARKER pointer
typedef unsigned software_marker_t;

#define SOFTWARE_MARK *((volatile software_marker_t *)(EXECUTION_TRACKER_BASE + 0))

#ifdef __cplusplus
extern "C" {
#endif
void sendchar(char ch);
void simulation_exit(void);
#ifdef __cplusplus
}
#endif

#endif
