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

.extern INSTRUCTION_ptr
.extern DATA_OUT_ptr;

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

    MOV R2, #0

    @ coluna: bits 3–5
    AND R3, R0, #0x07
    LSL R3, R3, #3
    ORR R2, R2, R3

    @ linha: bits 6–8
    AND R3, R1, #0x07
    LSL R3, R3, #6
    ORR R2, R2, R3

    @ opcode: bits 0–2 = 0b001
    ORR R2, R2, #0x1

    MOV R0, R2
    BL instruction

    @ Espera opcional por FLAG_DONE

    LDR R3, =DATA_OUT_ptr
    LDR R3, [R3]
    LDRSB R0, [R3]

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR


@ ------------------------- STORE (010)
@ R0 = valor
@ R1 = linha, R2 = coluna, R3 = matriz


store_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R4, #0

    @ valor: bits 10–17
    AND R5, R0, #0xFF       @ int8_t para 8 bits sem sinal
    LSL R5, R5, #10
    ORR R4, R4, R5

    @ linha: bits 7–9
    AND R5, R1, #0x07
    LSL R5, R5, #7
    ORR R4, R4, R5

    @ coluna: bits 4–6
    AND R5, R2, #0x07
    LSL R5, R5, #4
    ORR R4, R4, R5

    @ matriz: bit 3
    AND R5, R3, #0x01
    LSL R5, R5, #3
    ORR R4, R4, R5

    @ opcode: bits 0–2 = 0b010
    ORR R4, R4, #0x2

    @ Enviar instrução
    MOV R0, R4
    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR



@ ------------------------- ADD (011)
add_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #3           @ opcode = 011

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- SUB (100)
sub_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #4           @ opcode = 100

    BL instruction

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

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- MULM (110)
mult_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #6           @ opcode = 110

    BL instruction

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR

@ ------------------------- RST (111)
reset_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

    MOV R0, #7           @ opcode = 111

    BL instruction

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