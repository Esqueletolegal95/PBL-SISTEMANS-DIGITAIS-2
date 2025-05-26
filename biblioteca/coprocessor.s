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

.global map_fpga
.type map_fpga, %function

@ ------------------------- MMAP
map_fpga:
    PUSH {R4-R7, LR}          @ Salva registradores usados e LR

    @ open("/dev/mem", O_RDWR | O_SYNC)
    LDR R0, =DEV_MEM          @ pathname
    MOV R1, #0x2              @ O_RDWR = 0x2
    MOV R2, #0x1010           @ O_SYNC = 0x1010
    ORR R1, R1, R2            @ R1 = O_RDWR | O_SYNC = 0x1012
    BL open

    CMP R0, #0
    BLT map_error             @ se fd < 0, erro

    @ Salva file descriptor
    LDR R1, =FILE_DESCRIPTOR
    STR R0, [R1]

    @ mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BRIDGE)
    MOV R0, #0                @ NULL (addr)
    LDR R1, =FPGA_SPAM
    LDR R1, [R1]              @ length = 0x1000

    MOV R2, #0x3              @ PROT_READ | PROT_WRITE = 0x1 | 0x2 = 0x3
    MOV R3, #0x01             @ MAP_SHARED = 0x01

    LDR R4, =FILE_DESCRIPTOR
    LDR R4, [R4]              @ fd

    LDR R5, =FPGA_BRIDGE
    LDR R5, [R5]              @ offset = base físico 0xFF200000

    MOV R6, #0                @ offset high = 0 (para mmap syscall)

    BL mmap                   @ retorna ponteiro mapeado em R0

    CMP R0, #-1
    BEQ map_error             @ mmap falhou?

    @ Salva ponteiro base
    LDR R1, =FPGA_ADRS
    STR R0, [R1]

    @ Calcula ponteiros para registradores mapeados
    LDR R1, =INSTRUCTION_ptr
    STR R0, [R1]

    ADD R2, R0, #0x10
    LDR R1, =WR_ptr
    STR R2, [R1]

    ADD R2, R0, #0x20
    LDR R1, =DATA_OUT_ptr
    STR R2, [R1]

    ADD R2, R0, #0x30
    LDR R1, =FLAGS_ptr
    STR R2, [R1]


    POP {R4-R7, LR}
    BX LR

map_error:
    MOV R0, #0
    LDR R1, =FPGA_ADRS
    STR R0, [R1]

    LDR R1, =INSTRUCTION_ptr
    STR R0, [R1]
    LDR R1, =WR_ptr
    STR R0, [R1]
    LDR R1, =FLAGS_ptr
    STR R0, [R1]
    LDR R1, =DATA_OUT_ptr
    STR R0, [R1]

    POP {R4-R7, LR}
    BX LR

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

    LDR LR, [SP]
    ADD SP, SP, #4
    BX LR


@ ------------------------- STORE (010)
@ R0 = valor
@ R1 = linha, R2 = coluna, R3 = matriz
store_matrix:
    SUB SP, SP, #4
    STR LR, [SP]

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


.section .data

DEV_MEM: 
    .asciz "/dev/mem"             @ Caminho para /dev/mem

FPGA_SPAM:
    .word 0x1000                  @ Tamanho do mapeamento (4 KB)


FILE_DESCRIPTOR:
    .space 4                      @ File descriptor de /dev/mem

FPGA_BRIDGE:
    .word 0xFF200000              @ Endereço físico do bridge (base do mapeamento)

@ ---- Ponteiros para registradores mapeados (serão calculados após mmap)

INSTRUCTION_ptr:
    .space 4                      @ Ponteiro para registrador de instrução

WR_ptr:
    .space 4                      @ Ponteiro para registrador de escrita (WR)

FLAGS_ptr:
    .space 4                      @ Ponteiro para registrador de flags

DATA_OUT_ptr:
    .space 4                      @ Ponteiro para registrador de saída de dados