#ifndef PROTOTIPO_H
#define PROTOTIPO_H

// Mapeia os endereços de memória para serem compatíveis com o AXI bridge
void map_mem();

// Escreve valores em uma matriz.
// num1 e num2 são os dados (8 bits cada) de um elemento e do elemento seguinte e id identifica onde escrever(matriz, linha coluna).
void write_matrix(signed char num1, signed char num2, signed char id);

// Lê um valor de uma matriz usando o identificador.
// id usa 2 bits para a matriz, 3 bits para a linha e 3 bits para a coluna.
char read_matrix(signed char id);

// Multiplica uma matriz por um número escalar.
// num é um valor de 8 bits usado como escalar.
void mult_matrix_esc(signed char num);

// Soma duas matrizes e armazena o resultado em outra matriz.
void sum_matrix();

// Subtrai duas matrizes e armazena o resultado em outra matriz.
void sub_matrix();

// Multiplica duas matrizes e armazena o resultado em outra matriz.
void mult_matrix();

// Faz a transposição de uma matriz (troca linhas por colunas).
void trans_matrix();

// Faz a matriz oposta  da matrix_A (troca o sinal de todos os elementos).
void oppo_matrix();

// Calcula o determinante de matrix_A 2x2.
// num1 é a matriz de entrada, num2 pode ser usado para armazenar, id identifica.
void det2_matrix();

// Calcula o determinante de matrix_A 3x3.
void det3_matrix();

// Calcula o determinante de matrix_A 4x4.
void det4_matrix();

// Calcula o determinante de matrix_A 5x5.
void det5_matrix();

#endif
