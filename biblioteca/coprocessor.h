#ifndef COPROCESSOR_H
#define COPROCESSOR_H

#include <stdint.h>

/**
 * @file coprocessor.h
 * @brief Biblioteca para operações com matrizes via coprocessador FPGA.
 */

/**
 * @name Flags de status do coprocessador
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
        /* 1 byte de padding automático aqui */
    uint16_t flags;    /**< Flags de status do coprocessador (bitmask). */
} MatrixResult;

/**
 * @brief Executa uma instrução no coprocessador.
 * @param[in] instr Código da instrução a ser enviada.
 */
void instruction(uint32_t instr);

/**
 * @brief Executa a operação NOT (ou NOP) no coprocessador.
 * @return Flags de status da operação.
 */
uint16_t not_operation(void);

/**
 * @brief Lê um valor de uma posição específica de uma matriz.
 * @param[in] linha Índice da linha (0 a 4).
 * @param[in] coluna Índice da coluna (0 a 4).
 * @param[out] result Ponteiro para estrutura que armazenará valor e flags.
 * @return Flags de status da operação.
 */
uint16_t load_matrix(uint8_t linha, uint8_t coluna, MatrixResult *result);

/**
 * @brief Escreve um valor em uma posição de uma matriz.
 * @param[in] num Valor a ser armazenado.
 * @param[in] linha Índice da linha (0 a 4).
 * @param[in] coluna Índice da coluna (0 a 4).
 * @param[in] matrix Identificador da matriz (0 ou 1).
 * @return Flags de status da operação.
 */
uint16_t store_matrix(int8_t num, uint8_t linha, uint8_t coluna, uint8_t matrix);

/**
 * @brief Multiplica todos os elementos da matriz por um escalar.
 * @param[in] num Valor escalar (int8_t).
 * @return Flags de status da operação.
 */
uint16_t mult_matrix_esc(int8_t num);

/**
 * @brief Soma duas matrizes armazenadas no coprocessador.
 * @return Flags de status da operação.
 */
uint16_t add_matrix(void);

/**
 * @brief Subtrai duas matrizes armazenadas no coprocessador.
 * @return Flags de status da operação.
 */
uint16_t sub_matrix(void);

/**
 * @brief Multiplica duas matrizes armazenadas no coprocessador.
 * @return Flags de status da operação.
 */
uint16_t mult_matrix(void);

/**
 * @brief Reseta todas as matrizes no coprocessador.
 * @return Flags de status da operação.
 */
uint16_t reset_matrix(void);

#endif /* COPROCESSOR_H */
