.global instruction
.global write_matrix
.type write_matrix, %function
.global read_matrix
.type read_matrix, %function
.global sum_matrix
.type sum_matrix, %function
.global sub_matrix
.type sub_matrix, %function
.global mult_matrix
.type mult_matrix, %function
.global mult_matrix_esc
.type mult_matrix_esc, %function
.global trans_matrix
.type trans_matrix, %function
.global oppo_matrix
.type oppo_matrix, %function
.global det2_matrix
.type det2_matrix, %function
.global det3_matrix
.type det3_matrix, %function
.global det4_matrix
.type det4_matrix, %function
.global det5_matrix
.type det5_matrix, %function

read_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #1

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

write_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #20
    LSL R1, R1, #12
    LSL R2, R2, #4
    ADD R0, R0, R1
    ADD R0, R0, R2
    ADD R0, R0, #2

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

mult_matrix_esc:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #12
    LSL R1, R1, #4
    ADD R0, R0, R1
    ADD R0, R0, #3

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

sum_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #4

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

sub_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #5

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

mult_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #6

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

trans_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #7

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

oppo_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #8

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

det2_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #9

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

det3_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #10

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

det4_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #11

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

det5_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #4
    ADD R0, R0, #12

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

instruction:
    SUB SP, SP, #4
    STR R1, [SP]

    LDR R1, =FPGA_ADSS
    LDR R1, [R1]        
    STR R0, [R1]       

    LDR R1, [SP]
    ADD SP, SP, #4
    BX LR

.section .data

FPGA_ADSS:
    .word 0x00000000    @ Este valor deve ser setado pelo C com o mmap()
