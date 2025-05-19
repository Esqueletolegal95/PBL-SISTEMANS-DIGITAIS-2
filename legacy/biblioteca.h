#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdint.h>

// Escreve valores em uma matriz.
// num1 e num2 são os dados (8 bits cada) de um elemento e do elemento seguinte; id identifica onde escrever (matriz, linha, coluna).
void write_matrix(int8_t num1, int8_t num2, uint8_t id);

// Lê um valor de uma matriz usando o identificador.
// id usa 2 bits para a matriz, 3 bits para a linha e 3 bits para a coluna.
int8_t read_matrix(uint8_t id);

// Multiplica uma matriz por um número escalar.
// num é um valor de 8 bits usado como escalar.
void mult_matrix_esc(int8_t num);

// Soma duas matrizes e armazena o resultado em outra matriz.
void sum_matrix(void);

// Subtrai duas matrizes e armazena o resultado em outra matriz.
void sub_matrix(void);

// Multiplica duas matrizes e armazena o resultado em outra matriz.
void mult_matrix(void);

// Faz a transposição de uma matriz (troca linhas por colunas).
void trans_matrix(void);

// Calcula a matriz oposta de matrix_A (troca o sinal de todos os elementos).
void oppo_matrix(void);

// Calcula o determinante de matrix_A 2x2 e retorna o resultado.
int det2_matrix(void);

// Calcula o determinante de matrix_A 3x3 e retorna o resultado.
int det3_matrix(void);

// Calcula o determinante de matrix_A 4x4 e retorna o resultado.
int det4_matrix(void);

// Calcula o determinante de matrix_A 5x5 e retorna o resultado.
int det5_matrix(void);

#endif // BIBLIOTECA_H
