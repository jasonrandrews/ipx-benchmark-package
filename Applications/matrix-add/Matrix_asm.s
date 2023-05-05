;==================================================================
; Performance Monitoring Unit (PMU) Example Code for Cortex-A/R
;
; Copyright ARM Ltd 2010-2012. All rights reserved.
;==================================================================
	AREA matrix_preloader,CODE,READONLY

	ENTRY


	EXPORT add_matrix_in_ASM_optimized
add_matrix_in_ASM_optimized PROC ;treats the matrices as groups of 4 elements and does vector additions
    PUSH {r0-r12}
	MOV r12, lr
start_adding
	VLDMIA r1!, {q0}     ;Loads 4 elements from matrix 1 in a NEON Q register (128 bits)
	VLDMIA r2!, {q1}     ;Loads 4 elements from matrix 2 in a NEON Q register (128 bits)
	ADD r3, r3, #16
	MOV r4, r3
	VADD.I32 q2, q0, q1  ;Performs vector addition for 4 elements
	VSTMDB r4!, {q2}     ;Stores the result of the addition
	SUB r0, r0, #4
	CMP r0, #4
	BGE start_adding
	CMP r0, #0
	BEQ exit

add_the_last_elements    ;Adds the last elements of the array that do not fill an entire 4 element vector
	LDR r5, [r1], #4
	LDR r6, [r2], #4
	ADD r5, r5, r6
	STR r5, [r3], #4
	SUB r0, r0, #1
	CMP r0, #0
	BNE add_the_last_elements
	B exit
	ENDP

exit

	MOV lr, r12
	POP {r0-r12}
	BX lr

	END
