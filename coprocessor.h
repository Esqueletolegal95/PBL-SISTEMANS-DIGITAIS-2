#ifndef COPROCESSOR_H
#define COPROCESSOR_H

#include <stdint.h>

#define FLAG_DONE            (1 << 0)  /**< Bit 0: operação concluída com sucesso. */
#define FLAG_OVERFLOW        (1 << 1)  /**< Bit 1: estouro de valor durante a operação. */
#define FLAG_INCORRECT_ADDR  (1 << 2)  /**< Bit 2: endereço inválido acessado. */

/*Indica a não realização de operações pelo processador*/
void not_operation(void);

/* Escreve valores em uma matriz (A ou B).
 *num: valor a ser escrito na matrix
 *linha: linha da matriz onde o valor será escrito(0 a 4)
 *coluna: colina da matriz onde o valor será escrito(0 a 4)
 *matrix: bit que indica a matríx(0 ou 1)
*/

void store_matrix(int8_t num, uint8_t linha, uint8_t coluna, uint8_t matrix);

/*Carrega o valor desejado da matriz C para data_out
 *linha: linha da matriz onde o valor será carregado(0 a 4)
 *coluna: colina da matriz onde o valor será carregado(0 a 4)
 *retorno: valor de data_out
*/

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
