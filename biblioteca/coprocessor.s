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

@ ------------------------- wait_done()
@ Aguarda FLAGS[0] == 1 (bit "DONE")
wait_done:
    LDR R1, =FLAGS_ptr
    LDR R1, [R1]
wait_loop:
    LDRH R2, [R1]
    TST R2, #1
    BEQ wait_loop
    BX LR

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
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR

@ ------------------------- load_matrix()
@ R0 = linha, R1 = coluna, R2 = ponteiro MatrixResult
load_matrix:
    PUSH {LR}

    LSL R0, R0, #6       @ linha << 6
    LSL R1, R1, #3       @ coluna << 3
    ADD R3, R0, R1
    ADD R3, R3, #1       @ opcode 001

    MOV R0, R3
    BL instruction
    BL wait_done

    LDR R3, =DATA_OUT_ptr
    LDR R3, [R3]
    LDRSB R1, [R3]       @ valor da matriz

    LDR R3, =FLAGS_ptr
    LDR R3, [R3]
    LDRH R3, [R3]        @ flags

    STRB R1, [R2]        @ MatrixResult.value
    STRH R3, [R2, #2]    @ MatrixResult.flags

    MOV R0, R3           @ retorna flags
    POP {LR}
    BX LR

@ ------------------------- store_matrix()
@ R0 = valor, R1 = linha, R2 = coluna, R3 = matriz
store_matrix:
    PUSH {LR}

    LSL R4, R0, #10      @ valor << 10
    LSL R5, R1, #7       @ linha << 7
    ADD R4, R4, R5

    LSL R5, R2, #4       @ coluna << 4
    ADD R4, R4, R5

    LSL R5, R3, #3       @ matriz << 3
    ADD R4, R4, R5

    ADD R0, R4, #2       @ opcode = 010
    LDR R4, =WR_ptr
    LDR R4, [R4]
    MOV R5, #1
    STRB R5, [R4]        @ WR = 1

    BL instruction
    BL wait_done

    MOV R5, #0
    STRB R5, [R4]        @ WR = 0

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]        @ retorna flags

    POP {LR}
    BX LR

@ ------------------------- add_matrix()
add_matrix:
    MOV R0, #3
    BL instruction
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR

@ ------------------------- sub_matrix()
sub_matrix:
    MOV R0, #4
    BL instruction
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR

@ ------------------------- mult_matrix_esc()
@ R0 = escalar
mult_matrix_esc:
    LSL R0, R0, #3
    ADD R0, R0, #5
    BL instruction
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR

@ ------------------------- mult_matrix()
mult_matrix:
    MOV R0, #6
    BL instruction
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR

@ ------------------------- reset_matriz()
reset_matriz:
    MOV R0, #7
    BL instruction
    BL wait_done

    LDR R0, =FLAGS_ptr
    LDR R0, [R0]
    LDRH R0, [R0]
    BX LR
