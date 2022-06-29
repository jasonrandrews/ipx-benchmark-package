/*---------------------------------------------------------------*/
/* Performance Monitoring Unit (PMU) Example Code for Cortex-A/R */
/*                                                               */
/* Copyright (C) ARM Limited, 2010-2012. All rights reserved.    */
/*---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

extern void start_marker();
extern void stop_marker();

#define WIDTH 100
#define HEIGHT 100

int mat1[HEIGHT][WIDTH];
int mat2[HEIGHT][WIDTH];
int mat3[HEIGHT][WIDTH];

int number_of_elements = WIDTH * HEIGHT;
int *memory_location_mat1, *memory_location_mat2, *memory_location_mat3;

extern void add_matrix_in_ASM_optimized(int number_of_elements, int* pa, int* pb, int* pc);

int fill_matrix(int matnb) {
  int i, j;
  if (matnb == 1) {
    for (i = 0; i < HEIGHT; i++) {
      for (j = 0; j < WIDTH; j++) {
        mat1[i][j] = rand() % 100;
      }
    }
  }
  if (matnb == 2) {
    for (i = 0; i < HEIGHT; i++) {
      for (j = 0; j < WIDTH; j++) {
        mat2[i][j] = rand() % 100;
      }
    }
  }
  return 0;
}

int add_matrix_in_C_unoptimized(int mat1nb, int mat2nb) {
  if (mat1nb == 1 && mat2nb == 2) {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
      for (j = 0; j < WIDTH; j++) {
        mat3[i][j] = mat1[i][j] + mat2[i][j];  // Adds Matrix 1 and 2 one element at a time
      }
    }
  }
  return 0;
}

int main() {
  memory_location_mat1 = &mat1[0][0];  // Pointer to the first element of the array holding matrix 1
  memory_location_mat2 = &mat2[0][0];  // Pointer to the first element of the array holding matrix 2
  memory_location_mat3 =
      &mat3[0][0];  // Pointer to the first element of the array holding matrix 3 (result storage matrix)

  fill_matrix(1);  // Fill matrix 1 with random numbers
  fill_matrix(2);  // Fill matrix 2 with random numbers

  (void)start_marker();

  add_matrix_in_C_unoptimized(1, 2);
  // add_matrix_in_ASM_optimized(number_of_elements, memory_location_mat1, memory_location_mat2, memory_location_mat3);

  (void)stop_marker();

  return 0;
}
