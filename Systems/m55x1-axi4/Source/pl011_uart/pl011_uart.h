/* -----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited.
 *
 *               (C) COPYRIGHT 2012,2022 ARM Limited.
 *                   ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited.
 *
 * This file was adapted from the Kits KVS repo:
 * kvs/share/lib/kit_ms2r_lib/app_v8/include/pl011_uart.h
 *
 * -----------------------------------------------------------------------------
 * Purpose : PL011 r1p5 Driver
 *
 * -----------------------------------------------------------------------------
 */

#ifndef __PL011_UART_H__
#define __PL011_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------------
 * PL011 registers mask
 * -----------------------------------------------------------------------------
 */
enum UARTFR_BITS {
  UARTFR_TXFIFOEMPTY    = 1<<7,
  UARTFR_RXFIFOFULL     = 1<<6,
  UARTFR_TXFIFOFULL     = 1<<5,
  UARTFR_RXFIFOEMPTY    = 1<<4,
  UARTFR_TXFIFONOTEMPTY = 1<<3
};


/* -----------------------------------------------------------------------------
 * PL011 registers
 * -----------------------------------------------------------------------------
 */
typedef struct pl011_uart_t {
    volatile unsigned int Data;
    volatile unsigned int RSR_ECR;
    volatile unsigned int zReserved008_014[4];
    volatile unsigned int Flags;
    volatile unsigned int zReserved01c;
    volatile unsigned int IPLR;
    volatile unsigned int IBRD;
    volatile unsigned int FBRD;
    volatile unsigned int LCR_H;
    volatile unsigned int Ctrl;
    volatile unsigned int IFLS;
    volatile unsigned int MSC;
    volatile unsigned int RIS;
    volatile unsigned int MIS;
    volatile unsigned int IRQClear;
    volatile unsigned int DMACR;
    volatile unsigned int zReserved080_FDC[(0xFE0-0x80)/4];
    volatile unsigned int PeriphID[4];
    volatile unsigned int CellID[4];
} pl011_uart_t;

/* -----------------------------------------------------------------------------
 * PL011 functions
 * -----------------------------------------------------------------------------
 */

/* Function prototype - to initialize UART */
extern void pl011_uart_init(void*);

/* Function prototype - to get character from UART */
extern char pl011_uart_getchar_polled(void*);

/* Function prototype - to send character from UART */
extern void pl011_uart_putc_polled(void*, char);


#ifdef __cplusplus
}
#endif

#endif /* __PL011_UART_H__ */
