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
 * This file was adapted from the following files in the Kits KVS repo:
 * kvs/share/lib/kit_ms2r_lib/app_v8/src/pl011_uart.c
 * kvs/share/lib/kit_mpg_lib/common/uart.c
 *
 * -----------------------------------------------------------------------------
 * Purpose : PL011 r1p5 Driver
 *
 * -----------------------------------------------------------------------------
 */

#include "pl011_uart.h"
#include <stdint.h>

/**
@brief UART Init Function

Initialize UART for transmit and receive data

@param pl011_uart_ptr:  UART base address
@return  Nil
*/
void pl011_uart_init(void* pl011_uart_ptr) {
    pl011_uart_t* uart = (pl011_uart_t*) pl011_uart_ptr;

    // Clear the interrupt status bits as a part of initialization
    // Set the baud rate to work in sync with the DUT present.
    // Selects the character length, parity, FIFO enable etc. for receiving characters to print
    // Enable the UART with transmit only, receive is not needed.
    uart->RSR_ECR = ((uint8_t)0xFF);
    uart->IBRD = 1;
    uart->FBRD = 0;
    uart->LCR_H = ((uint16_t)0x0060) | ((uint16_t)0x0010);
    uart->Ctrl = ((uint16_t)0x0001) | ((uint16_t)0x0100);
}

/**
@brief UART Read Function

Wait till UART receives data & return the data.
@param pl011_uart_ptr:  UART base address
@return  Read data from the receive FIFO
*/
char pl011_uart_getchar_polled(void* pl011_uart_ptr) {
    pl011_uart_t* uart = (pl011_uart_t*) pl011_uart_ptr;

    while (uart->Flags & UARTFR_RXFIFOEMPTY)
        continue;

    return uart->Data;
}

/**
@brief UART Write Function

Wait till UART TX FIFO becomes free and write the data to FIFO
@param pl011_uart_ptr:  UART base address
@param c : character to write
@return  Nil
*/
void pl011_uart_putc_polled(void* pl011_uart_ptr, char c) {
    pl011_uart_t* uart = (pl011_uart_t*) pl011_uart_ptr;

    while (uart->Flags & UARTFR_TXFIFOFULL)
        continue;

    uart->Data = c;
}
