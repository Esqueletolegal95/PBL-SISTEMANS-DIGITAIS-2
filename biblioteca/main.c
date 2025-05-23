#include <stdio.h>
#include "coprocessor.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "./hps_0.h"

#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x1000
volatile uint32_t *INSTRUCTION_ptr;
volatile uint32_t *FLAGS_ptr;
volatile uint32_t *WR_ptr;
volatile uint32_t *DATA_OUT_ptr;

#define CHECK_FLAGS(f) do { \
    if ((f) & FLAG_DONE) { printf("✅ Operação concluída com sucesso!\n"); } \
    if ((f) & FLAG_OVERFLOW) { printf("⚠️ Overflow detectado!\n"); } \
    if ((f) & FLAG_INCORRECT_ADDR) { printf("❌ Endereço incorreto acessado!\n"); } \
} while(0)

int init_fpga_mapping() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    void *LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    INSTRUCTION_ptr = (volatile uint32_t *)(LW_virtual + INSTRUCTION_PIO_0_BASE);
    FLAGS_ptr = (volatile uint32_t *)(LW_virtual + FLAGS_PIO_0_BASE);
    WR_ptr = (volatile uint32_t *)(LW_virtual + WR_PIO_0_BASE);
    DATA_OUT_ptr = (volatile uint32_t *)(LW_virtual + DATA_OUT_PIO_0_BASE);
    close(fd);
    return 0;
}

void preencher_matriz_teste() {
    printf("✨ Preenchendo matriz 0 com valores de teste...\n");

    for (uint8_t linha = 0; linha < 5; linha++) {
        for (uint8_t coluna = 0; coluna < 5; coluna++) {
            int8_t valor = linha * 5 + coluna;  // padrão simples só pra ver que tá tudo certo
            *WR_ptr = 1;
            store_matrix(valor, linha, coluna, 0);
            *WR_ptr = 0;
        }
    }

    printf("✔️ Matriz preenchida com sucesso! Agora use a opção 9 pra ver\n");
}

void print_matrix(int tamanho) {
    MatrixResult res;
    printf("Conteúdo da matriz 0:\n");
    for (uint8_t i = 0; i < tamanho; i++) {
        for (uint8_t j = 0; j < tamanho; j++) {
            load_matrix(i, j, &res);
            int8_t valor = *(int8_t *)DATA_OUT_ptr;
            printf("%4d ", valor);
        }
        printf("\n");
    }
}

uint16_t read_element() {
    MatrixResult res;
    uint8_t linha, coluna;
    printf("Linha da matriz (0 a 4):\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4):\n");
    scanf("%hhu", &coluna);
    load_matrix(linha, coluna, &res);
    return res.flags;
}

uint16_t write_elements() {
    uint8_t matriz_id, linha, coluna;
    int8_t num;
    printf("Id da matriz (0 a 1):\n");
    scanf("%hhu", &matriz_id);
    printf("Linha da matriz (0 a 4):\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4):\n");
    scanf("%hhu", &coluna);
    printf("Elemento a ser escrito:\n");
    scanf("%hhd", &num);
    *WR_ptr = 1;
    uint16_t flags = store_matrix(num, linha, coluna, matriz_id);
    *WR_ptr = 0;
    return flags;
}

void menu() {
    while (1) {
        int opcao;
        printf("\nEscolha uma opcao:\n");
        printf("1 - Ler elemento na matriz\n");
        printf("2 - Escrever elementos na matriz\n");
        printf("3 - Multiplicar matriz por um escalar\n");
        printf("4 - Somar matrizes\n");
        printf("5 - Subtrair matrizes\n");
        printf("6 - Multiplicar matrizes\n");
        printf("7 - Mostrar flags\n");
        printf("8 - Mostrar saída\n");
        printf("9 - Imprimir matriz\n");
        printf("10 - Gerar matriz teste\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        uint16_t flags = 0;

        switch (opcao) {
            case 1: {
                flags = read_element();
                int8_t valor = *(int8_t*)DATA_OUT_ptr;
                printf("Valor lido: %d\n", valor);
                CHECK_FLAGS(flags);
                break;
            }
            case 2: {
                flags = write_elements();
                CHECK_FLAGS(flags);
                break;
            }
            case 3: {
                int8_t num;
                printf("Digite o escalar:\n");
                scanf("%hhd", &num);
                flags = mult_matrix_esc(num);
                CHECK_FLAGS(flags);
                break;
            }
            case 4: {
                flags = add_matrix();
                CHECK_FLAGS(flags);
                break;
            }
            case 5: {
                flags = sub_matrix();
                CHECK_FLAGS(flags);
                break;
            }
            case 6: {
                flags = mult_matrix();
                CHECK_FLAGS(flags);
                break;
            }
            case 7: {
                flags = *(uint16_t*)FLAGS_ptr;
                printf("Flags atuais: 0x%04X\n", flags);
                CHECK_FLAGS(flags);
                break;
            }
            case 8: {
                int8_t val = *(int8_t*)DATA_OUT_ptr;
                printf("Saída: %d\n", val);
                flags = *(uint16_t*)FLAGS_ptr;
                CHECK_FLAGS(flags);
                break;
            }
            case 9: {
                print_matrix(5);
                break;
            }
            case 10: {
                preencher_matriz_teste();
                break;
            }
            default:
                printf("Opção inválida, tente novamente.\n");
                break;
        }
    }
}

int main() {
    if (init_fpga_mapping() != 0) {
        printf("Erro ao mapear FPGA.\n");
        return -1;
    }

    menu(); // loop do menu
    return 0;
}
