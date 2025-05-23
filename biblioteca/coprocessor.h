#ifndef COPROCESSOR_H
#define COPROCESSOR_H

#include <stdint.h>

/**
 * @file coprocessor.h
 * @brief Biblioteca para operações com matrizes via coprocessador FPGA.
 *
 * Esta biblioteca provê funções para manipulação de matrizes armazenadas
 * no coprocessador, incluindo leitura, escrita, operações aritméticas
 * e reset das matrizes. As operações são realizadas via interface com o HPS.
 */

/**
 * @name Flags de status do coprocessador
 * @brief Máscaras de bits para interpretação dos flags de status.
 * @{
 */
#define FLAG_DONE            (1 << 0)  /**< Bit 0: operação concluída com sucesso. */
#define FLAG_OVERFLOW        (1 << 1)  /**< Bit 1: estouro de valor durante a operação. */
#define FLAG_INCORRECT_ADDR  (1 << 2)  /**< Bit 2: endereço inválido acessado. */
/** @} */

/**
 * @brief Resultado de uma leitura de matriz contendo valor e flags de status.
 */
typedef struct {
    int8_t value;      /**< Valor lido da matriz. */
    uint16_t flags;    /**< Flags de status do coprocessador (bitmask). */
} MatrixResult;

/**
 * @brief Escreve um valor em uma posição específica de uma matriz.
 *
 * Esta função armazena o valor `num` na matriz identificada por `matrix`
 * na posição indicada por `linha` e `coluna`.
 *
 * @param[in] num Valor a ser armazenado (int8_t).
 * @param[in] linha Índice da linha na matriz (0 a 4).
 * @param[in] coluna Índice da coluna na matriz (0 a 4).
 * @param[in] matrix Identificador da matriz (0 ou 1).
 * @return Flags de status da operação (bitmask).
 */
uint16_t store_matrix(int8_t num, uint8_t linha, uint8_t coluna, uint8_t matrix);

/**
 * @brief Lê um valor de uma posição específica de uma matriz.
 *
 * Lê o valor armazenado na matriz na posição indicada por `linha` e `coluna`,
 * retornando também os flags de status.
 *
 * @param[in] linha Índice da linha na matriz (0 a 4).
 * @param[in] coluna Índice da coluna na matriz (0 a 4).
 * @return Estrutura MatrixResult com valor e flags.
 */
MatrixResult load_matrix(uint8_t linha, uint8_t coluna);

/**
 * @brief Multiplica todos os elementos da matriz por um escalar.
 *
 * Aplica uma multiplicação escalar em todos os elementos da matriz.
 *
 * @param[in] num Valor escalar para multiplicação (int8_t).
 * @return Flags de status da operação (bitmask).
 */
uint16_t mult_matrix_esc(int8_t num);

/**
 * @brief Soma duas matrizes armazenadas no coprocessador.
 *
 * Realiza a soma das matrizes configuradas no coprocessador e armazena
 * o resultado conforme especificado pela implementação interna.
 *
 * @return Flags de status da operação (bitmask).
 */
uint16_t add_matrix(void);

/**
 * @brief Subtrai duas matrizes armazenadas no coprocessador.
 *
 * Realiza a subtração das matrizes configuradas no coprocessador e armazena
 * o resultado conforme especificado pela implementação interna.
 *
 * @return Flags de status da operação (bitmask).
 */
uint16_t sub_matrix(void);

/**
 * @brief Multiplica duas matrizes armazenadas no coprocessador.
 *
 * Realiza a multiplicação das matrizes configuradas no coprocessador e armazena
 * o resultado conforme especificado pela implementação interna.
 *
 * @return Flags de status da operação (bitmask).
 */
uint16_t mult_matrix(void);

/**
 * @brief Reseta todas as matrizes no coprocessador.
 *
 * Zera ou reinicializa todas as matrizes armazenadas no coprocessador FPGA.
 *
 * @return Flags de status da operação (bitmask).
 */
uint16_t reset_matrix(void);

#endif /* COPROCESSOR_H */
