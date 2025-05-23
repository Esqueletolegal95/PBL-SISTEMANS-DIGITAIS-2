#include <stdint.h>

// Definição da estrutura do resultado da matriz (equivalente ao que a `load_matrix` retorna)
typedef struct {
    int8_t value;
    uint16_t flags;
} MatrixResult;

// Ponteiros globais para comunicação com o coprocessador (devem ser inicializados antes de uso)
volatile uint32_t *INSTRUCTION_ptr;
volatile uint16_t *FLAGS_ptr;
volatile uint8_t  *WR_ptr;
volatile int8_t   *DATA_OUT_ptr;

// ------------------------- instruction()
void instruction(uint32_t instr) {
    *INSTRUCTION_ptr = instr;
}

// ------------------------- not_operation()
void not_operation(void) {
    instruction(0);
}

// ------------------------- load_matrix()
uint16_t load_matrix(uint8_t linha, uint8_t coluna, MatrixResult *result) {
    uint32_t instr = ((linha & 0x07) << 6) | ((coluna & 0x07) << 3) | 0x01;
    instruction(instr);

    result->value = *DATA_OUT_ptr;
    result->flags = *FLAGS_ptr;
    return result->flags;
}

// ------------------------- store_matrix()
void store_matrix(int8_t valor, uint8_t linha, uint8_t coluna, uint8_t matriz) {
    uint32_t instr = ((uint32_t)(valor & 0xFF) << 10) |
                     ((linha & 0x07) << 7) |
                     ((coluna & 0x07) << 4) |
                     ((matriz & 0x07) << 3) |
                     0x02;

    *WR_ptr = 1;
    instruction(instr);
    *WR_ptr = 0;
}

// ------------------------- add_matrix()
void add_matrix(void) {
    instruction(0x03);
}

// ------------------------- sub_matrix()
void sub_matrix(void) {
    instruction(0x04);
}

// ------------------------- mult_matrix_esc()
void mult_matrix_esc(uint8_t escalar) {
    uint32_t instr = ((escalar & 0xFF) << 3) | 0x05;
    instruction(instr);
}

// ------------------------- mult_matrix()
void mult_matrix(void) {
    instruction(0x06);
}

// ------------------------- reset_matriz()
void reset_matriz(void) {
    instruction(0x07);
}
