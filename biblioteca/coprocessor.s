.global instruction
.type instruction, %function

.global load_matrix
.type load_matrix, %function

.global store_matrix
.type store_matrix, %function

.global add_matrix
.type add_matrix, %function

.global sub_matrix
.type sub_matrix, %function

.global mult_matrix
.type mult_matrix, %function

.global mult_matrix_esc
.type mult_matrix_esc, %function

.global reset_matrix
.type reset_matrix, %function

.global not_operation
.type not_operation, %function

.global wait_done
.type wait_done, %function

.extern INSTRUCTION_ptr

.extern FLAGS_ptr

.extern WR_ptr

.extern DATA_OUT_ptr

@ ------------------------- NOP (000)
not_operation:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #0          @ Só o opcode nos bits 0-2

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- LOAD (001)
@ R0 = coluna
@ R1 = linha
load_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #3      @ coluna << 3 → bits 3–5
    LSL R1, R1, #6      @ linha << 6  → bits 6–8
    ADD R0, R0, R1
    ADD R0, R0, #1      @ opcode = 001

    BL instruction

    LDR R1, =DATA_OUT_ptr
    LDR R1, [R1]
    LDRSB R0, [R1]

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR


@ ------------------------- STORE (010)
@ R0 = valor
@ R1 = linha, R2 = coluna, R3 = matriz
store_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    SXTB R0, R0       
    LSL R0, R0, #10       @ valor << 10
    LSL R1, R1, #7        @ linha << 7
    ADD R0, R0, R1

    LSL R2, R2, #4        @ coluna << 4
    ADD R0, R0, R2

    LSL R3, R3, #3        @ matriz << 3
    ADD R0, R0, R3

    ADD R0, R0, #2        @ opcode = 010 (STORE)

    @ WR = 1
    LDR R4, =WR_ptr
    LDR R4, [R4]      @ aqui carrega o valor do ponteiro
    MOV R5, #1
    STR R5, [R4]

    @ Executa a instrução
    BL instruction

    @ WR = 0
    LDR R4, =WR_ptr
    LDR R4, [R4]      @ carrega de novo o valor do ponteiro
    MOV R5, #0
    STR R5, [R4]
    
    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR


@ ------------------------- ADD (011)
add_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #3           @ opcode = 011

    BL instruction

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- SUB (100)
sub_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #4           @ opcode = 100

    BL instruction

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- MULE (101)
@ R0 = escalar (8 bits)
mult_matrix_esc:
    SUB SP, SP, #4
    STR LR, [SP]

    LSL R0, R0, #3       @ escalar nos bits 3–10
    ADD R0, R0, #5       @ opcode = 101

    BL instruction

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- MULM (110)
mult_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #6           @ opcode = 110

    BL instruction

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- RST (111)
reset_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #7           @ opcode = 111

    BL instruction

    BL wait_done

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- instruction()
instruction:
    SUB SP, SP, #4
    STR R1, [SP]

    LDR R1, =INSTRUCTION_ptr
    LDR R1, [R1]        
    STR R0, [R1]       

    LDR R1, [SP]
    ADD SP, SP, #4
    BX LR

wait_done:
    SUB SP, SP, #4
    STR R1, [SP]

wait_loop:
    LDR R1, =FLAGS_ptr
    LDR R0, [R1]
    AND R0, R0, #1
    CMP R0, #1

    BEQ wait_loop

    LDR R1, [SP]
    ADD SP, SP, #4
    
    BL not_operation

    LDR R0, [SP]
    LDR R1, [SP, #4]
    ADD SP, SP, #8
    BX LR 