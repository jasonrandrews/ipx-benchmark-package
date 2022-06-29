/*
** Copyright (c) 2019,2021 Arm Limited. All rights reserved.
*/

#ifndef __SERIAL_H
#define __SERIAL_H

#define SERIAL_BASE 0x1A200000
#define SERIAL_DATA *((volatile unsigned*)SERIAL_BASE)
#define SOFTWARE_MARK *((volatile unsigned*)(SERIAL_BASE + 4))
        
#ifdef __cplusplus
extern "C" {
#endif
void sendchar(char ch);
void simulation_exit(void);
#ifdef __cplusplus
}
#endif

#endif
