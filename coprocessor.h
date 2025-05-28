#ifndef COPROCESSOR_H
#define COPROCESSOR_H

#include <stdint.h>

#define FLAG_DONE            (1 << 0)  /**< Bit 0: operação concluída com sucesso. */
#define FLAG_OVERFLOW        (1 << 1)  /**< Bit 1: estouro de valor durante a operação. */
#define FLAG_INCORRECT_ADDR  (1 << 2)  /**< Bit 2: endereço inválido acessado. */

void not_operation(void);

// Escreve valores em uma matriz.
// id usa 1 bit para a matriz, 3 bits para a linha e 3 bits para a coluna.
void store_matrix(int8_t num, uint8_t linha, uint8_t coluna, uint8_t matrix);

// Lê um valor de uma matriz usando o identificador.
// id usa 3 bits para a coliuna e 3 bits para a linha.
int8_t load_matrix(uint8_t linha, uint8_t coluna);

// Multiplica uma matriz por um número escalar.
// num é um valor de 8 bits usado como escalar.
void mult_matrix_esc(int8_t num);

// Soma duas matrizes e armazena o resultado em outra matriz.
void add_matrix(void);

// Subtrai duas matrizes e armazena o resultado em outra matriz.
void sub_matrix(void);

// Multiplica duas matrizes e armazena o resultado em outra matriz.
void mult_matrix(void);

//Reseta todas as matrizes
void reset_matrix(void);
#endif
