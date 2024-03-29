/*
 * Whetstone.c
 *
 * 	1.	Convert original double precision benchmark into single precision benchmark.
 * 	2.	Get rid of Module 7: Trigonometric functions
 * 	3.  Change the loop count from 1000 to 40.
 * 	4. 	Get rid of the printf() functions.
 *
 *  Created on: Sep 22, 2017
 *      Author: Zhefeng Li
 *
 */

//////////////////////////////////////////////////////////////////
// Original copyright information is here.

/*
 * C Converted Whetstone Double Precision Benchmark
 *		Version 1.2	22 March 1998
 *
 *	(c) Copyright 1998 Painter Engineering, Inc.
 *		All Rights Reserved.
 *
 *		Permission is granted to use, duplicate, and
 *		publish this text and program as long as it
 *		includes this entire comment block and limited
 *		rights reference.
 *
 * Converted by Rich Painter, Painter Engineering, Inc. based on the
 * www.netlib.org benchmark/whetstoned version obtained 16 March 1998.
 *
 * A novel approach was used here to keep the look and feel of the
 * FORTRAN version.  Altering the FORTRAN-based array indices,
 * starting at element 1, to start at element 0 for C, would require
 * numerous changes, including decrementing the variable indices by 1.
 * Instead, the array E1[] was declared 1 element larger in C.  This
 * allows the FORTRAN index range to function without any literal or
 * variable indices changes.  The array element E1[0] is simply never
 * used and does not alter the benchmark results.
 *
 * The major FORTRAN comment blocks were retained to minimize
 * differences between versions.  Modules N5 and N12, like in the
 * FORTRAN version, have been eliminated here.
 *
 * An optional command-line argument has been provided [-c] to
 * offer continuous repetition of the entire benchmark.
 * An optional argument for setting an alternate LOOP count is also
 * provided.  Define PRINTOUT to cause the POUT() function to print
 * outputs at various stages.  Final timing measurements should be
 * made with the PRINTOUT undefined.
 *
 * Questions and comments may be directed to the author at
 *			r.painter@ieee.org
 */

extern void start_marker();
extern void stop_marker();

// Global variables to mark the start and end of the C_Entry() function.
// They are set in InitServoCortexR8.s
int g_iEntryStartMark = 0;
int g_iEntryEndMark = 0;

/* function prototypes */
void PA(float E[]);
void P0(void);
void P3(float X, float Y, float* Z);
/*
        COMMON T,T1,T2,E1(4),J,K,L
*/
float T, T1, T2, E1[5];
int J, K, L;

int main(void) {
  /* used in the FORTRAN version */
  long I;
  long N1, N2, N3, N4, N6, N8, N9, N10;
  float X1, X2, X3, X4, X, Y, Z;
  long LOOP;

  /* added for this version */
  long loopstart;

  loopstart = 40; /* see the note about LOOP below */

  /*
  C
  C	The actual benchmark starts here.
  C
  */
  (void)start_marker();

  T = .499975;
  T1 = 0.50025;
  T2 = 2.0;
  /*
  C
  C	With loopcount LOOP=10, one million Whetstone instructions
  C	will be executed in EACH MAJOR LOOP..A MAJOR LOOP IS EXECUTED
  C	'II' TIMES TO INCREASE WALL-CLOCK TIMING ACCURACY.
  C
          LOOP = 1000;
  */
  LOOP = loopstart;

  N1 = 0;
  N2 = 12 * LOOP;
  N3 = 14 * LOOP;
  N4 = 345 * LOOP;
  N6 = 210 * LOOP;

  N8 = 899 * LOOP;
  N9 = 616 * LOOP;
  N10 = 0;
  /*
  C
  C	Module 1: Simple identifiers
  C
  */
  X1 = 1.0;
  X2 = -1.0;
  X3 = -1.0;
  X4 = -1.0;

  for (I = 1; I <= N1; I++) {
    X1 = (X1 + X2 + X3 - X4) * T;
    X2 = (X1 + X2 - X3 + X4) * T;
    X3 = (X1 - X2 + X3 + X4) * T;
    X4 = (-X1 + X2 + X3 + X4) * T;
  }

  /*
  C
  C	Module 2: Array elements
  C
  */
  E1[1] = 1.0;
  E1[2] = -1.0;
  E1[3] = -1.0;
  E1[4] = -1.0;

  for (I = 1; I <= N2; I++) {
    E1[1] = (E1[1] + E1[2] + E1[3] - E1[4]) * T;
    E1[2] = (E1[1] + E1[2] - E1[3] + E1[4]) * T;
    E1[3] = (E1[1] - E1[2] + E1[3] + E1[4]) * T;
    E1[4] = (-E1[1] + E1[2] + E1[3] + E1[4]) * T;
  }

  /*
  C
  C	Module 3: Array as parameter
  C
  */
  for (I = 1; I <= N3; I++)
    PA(E1);

  /*
  C
  C	Module 4: Conditional jumps
  C
  */
  J = 1;
  for (I = 1; I <= N4; I++) {
    if (J == 1)
      J = 2;
    else
      J = 3;

    if (J > 2)
      J = 0;
    else
      J = 1;

    if (J < 1)
      J = 1;
    else
      J = 0;
  }

  /*
  C
  C	Module 5: Omitted
  C 	Module 6: Integer arithmetic
  C
  */

  J = 1;
  K = 2;
  L = 3;

  for (I = 1; I <= N6; I++) {
    J = J * (K - J) * (L - K);
    K = L * K - (L - J) * K;
    L = (L - K) * (K + J);
    E1[L - 1] = J + K + L;
    E1[K - 1] = J * K * L;
  }

  /*
  C
  C	Module 8: Procedure calls
  C
  */
  X = 1.0;
  Y = 1.0;
  Z = 1.0;

  for (I = 1; I <= N8; I++)
    P3(X, Y, &Z);

  /*
  C
  C	Module 9: Array references
  C
  */
  J = 1;
  K = 2;
  L = 3;
  E1[1] = 1.0;
  E1[2] = 2.0;
  E1[3] = 3.0;

  for (I = 1; I <= N9; I++)
    P0();

  /*
  C
  C	Module 10: Integer arithmetic
  C
  */
  J = 2;
  K = 3;

  for (I = 1; I <= N10; I++) {
    J = J + K;
    K = J + K;
    J = K - J;
    K = K - J - J;
  }

  /*
  C
  C      THIS IS THE END OF THE MAJOR LOOP.
  C
  */
  (void)stop_marker();
  return 0;
}

void PA(float E[]) {
  J = 0;

L10:
  E[1] = (E[1] + E[2] + E[3] - E[4]) * T;
  E[2] = (E[1] + E[2] - E[3] + E[4]) * T;
  E[3] = (E[1] - E[2] + E[3] + E[4]) * T;
  E[4] = (-E[1] + E[2] + E[3] + E[4]) / T2;
  J += 1;

  if (J < 6)
    goto L10;
}

void P0(void) {
  E1[J] = E1[K];
  E1[K] = E1[L];
  E1[L] = E1[J];
}

void P3(float X, float Y, float* Z) {
  float X1, Y1;

  X1 = X;
  Y1 = Y;
  X1 = T * (X1 + Y1);
  Y1 = T * (X1 + Y1);
  *Z = (X1 + Y1) / T2;
}
