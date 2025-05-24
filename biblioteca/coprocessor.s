.global instruction
.type instruction, %function

.global not_operation
.type not_operation, %function

.global load_matrix
.type load_matrix, %function

.global store_matrix
.type store_matrix, %function

.global add_matrix
.type add_matrix, %function

.global sub_matrix
.type sub_matrix, %function

.global mult_matrix_esc
.type mult_matrix_esc, %function

.global mult_matrix
.type mult_matrix, %function

.global reset_matriz
.type reset_matriz, %function

.extern INSTRUCTION_ptr
.extern FLAGS_ptr
.extern WR_ptr
.extern DATA_OUT_ptr

@ ------------------------- instruction()
instruction:
    LDR R1, =INSTRUCTION_ptr
    LDR R1, [R1]
    STR R0, [R1]
    BX LR

@ ------------------------- not_operation()
not_operation:
    MOV R0, #0
    BL instruction

wait_done_not:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_not

    MOV R0, R2
    BX LR

@ ------------------------- load_matrix()
@ R0 = linha, R1 = coluna, R2 = ponteiro MatrixResult
load_matrix:
    PUSH {LR}

    LSL R3, R0, #6          @ linha << 6
    LSL R4, R1, #3          @ coluna << 3
    MOV R5, #1              @ opcode 001
    ORR R3, R3, R4
    ORR R3, R3, R5          @ opcode bits 0-2 = 1

    MOV R0, R3
    BL instruction

wait_done_load:
    LDR R6, =FLAGS_ptr
    LDR R6, [R6]
    LDRH R7, [R6]
    AND R7, R7, #1
    CMP R7, #0
    BEQ wait_done_load

    LDR R6, =DATA_OUT_ptr
    LDR R6, [R6]
    LDRSB R4, [R6]

    LDR R6, =FLAGS_ptr
    LDR R6, [R6]
    LDRH R5, [R6]

    STRB R4, [R2]
    STRH R5, [R2, #1]

    MOV R0, R5
    POP {LR}
    BX LR

@ ------------------------- store_matrix()
@ R0 = valor (int8_t), R1 = linha (0-4), R2 = coluna (0-4), R3 = matriz (0 ou 1)
store_matrix:
    PUSH {LR}

    AND R4, R0, #0xFF        @ valor 8 bits
    LSL R4, R4, #10          @ valor << 10 (bits 10-17)

    LSL R5, R1, #7           @ linha << 7 (bits 7-9)
    ORR R4, R4, R5

    LSL R5, R2, #4           @ coluna << 4 (bits 4-6)
    ORR R4, R4, R5

    LSL R5, R3, #3           @ matriz << 3 (bit 3)
    ORR R4, R4, R5

    ORR R4, R4, #2           @ opcode 010

    MOV R0, R4

    LDR R5, =WR_ptr
    LDR R5, [R5]
    MOV R6, #1
    STRB R6, [R5]            @ WR = 1

    BL instruction

wait_done_store:
    LDR R7, =FLAGS_ptr
    LDR R7, [R7]
    LDRH R8, [R7]
    AND R8, R8, #1
    CMP R8, #0
    BEQ wait_done_store

    MOV R6, #0
    STRB R6, [R5]            @ WR = 0

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]

    POP {LR}
    BX LR

@ ------------------------- add_matrix()
add_matrix:
    MOV R0, #3               @ opcode 011
    BL instruction

wait_done_add:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_add

    MOV R0, R2
    BX LR

@ ------------------------- sub_matrix()
sub_matrix:
    MOV R0, #4               @ opcode 100
    BL instruction

wait_done_sub:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_sub

    MOV R0, R2
    BX LR

@ ------------------------- mult_matrix_esc()
@ R0 = escalar
mult_matrix_esc:
    LSL R0, R0, #3          @ escalar << 3
    ORR R0, R0, #5          @ opcode 101
    BL instruction

wait_done_mult_esc:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_mult_esc

    MOV R0, R2
    BX LR

@ ------------------------- mult_matrix()
mult_matrix:
    MOV R0, #6               @ opcode 110
    BL instruction

wait_done_mult:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_mult

    MOV R0, R2
    BX LR

@ ------------------------- reset_matriz()
reset_matriz:
    MOV R0, #7               @ opcode 111
    BL instruction

wait_done_reset:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
    LDRH R2, [R1]
    AND R2, R2, #1
    CMP R2, #0
    BEQ wait_done_reset

    MOV R0, R2
    BX LR
