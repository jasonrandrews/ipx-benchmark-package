/*
** Copyright (c) 2019 Arm Limited. All rights reserved.
*/

#include "serial.h"
#include <stdio.h>

// GCC ONLY usage
int _write(int fd, const void* buf, size_t count) {
  (void)fd; /* Parameter is not used, suppress unused argument warning */
  int sendcount = 0;
  for (; count > 0; --count) {
    sendchar((char)*(char*)buf);
    (char*)buf++; /* Increase buffer pointer */
    sendcount++;  /* Increase char counter */
  }

  return sendcount;
}

void simulation_exit() {
  sendchar(4);
}

void sendchar(char ch) {
  SERIAL_DATA = ch;
  if (ch == '\n') {
    ch = '\r';
    sendchar(ch);
  }
}

void software_start() {
  SOFTWARE_MARK = 1;
}

void software_stop() {
  SOFTWARE_MARK = 0;
}

// ReTargeting Code
// ================
#ifdef SERIAL

asm("  .global __use_no_semihosting\n");

struct __FILE {
  int handle;
};
FILE __stdout;
FILE __stdin;
FILE __stderr;

int fputc(int ch, FILE* f) {
  char tempch = ch;
  (void)sendchar(tempch);
  return ch;
}

void _ttywrch(int ch) {
  char tempch = ch;
  (void)sendchar(tempch);
}

// AC6 re-route
void _sys_exit(int return_code) {
  (void)simulation_exit();  // Stops simulation by writing a char of '4' to the trickbox
}
// GCC re-route
void _exit(int status) {
  (void)simulation_exit();  // Stops simulation by writing a char of '4' to the trickbox
  while (1)
    ;
}

int clock() {
  return (0);
}

#endif
