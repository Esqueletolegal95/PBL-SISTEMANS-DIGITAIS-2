#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "./hps_0.h"
#include "coprocessor.h"

#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x1000
#define MATRIX_SIZE 5

volatile uint32_t *INSTRUCTION_ptr;
volatile uint32_t *FLAGS_ptr;
volatile uint32_t *WR_ptr;
volatile uint32_t *DATA_OUT_ptr;

#define CHECK_FLAGS(f) do { \
    if ((f) & FLAG_DONE) printf("✅ Operação concluída com sucesso!\n"); \
    if ((f) & FLAG_OVERFLOW) printf("⚠️ Overflow detectado!\n"); \
    if ((f) & FLAG_INCORRECT_ADDR) printf("❌ Endereço incorreto acessado!\n"); \
} while (0)

volatile bool running = true;

void* monitor_flags(void *arg) {
    uint16_t last_flags = 0;

    while (running) {
        uint16_t current_flags = *(uint16_t*)FLAGS_ptr;

        if (current_flags != last_flags && current_flags != 0) {
            printf("\n📍 [Thread Flags] Flags alteradas: 0x%04X\n", current_flags);
            CHECK_FLAGS(current_flags);
            last_flags = current_flags;
        }

        usleep(200000); // 200ms
    }

    return NULL;
}

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

    for (int linha = 0; linha < MATRIX_SIZE; linha++) {
        for (int coluna = 0; coluna < MATRIX_SIZE; coluna++) {
            int8_t valor = linha * MATRIX_SIZE + coluna;
            store_matrix(valor, linha, coluna, 0);
        }
    }

    printf("✔️ Matriz preenchida com sucesso! Agora use a opção 9 pra ver\n");
}

void print_matrix(int tamanho) {
    printf("Conteúdo da matriz 0:\n");
    for (uint8_t i = 0; i < tamanho; i++) {
        for (uint8_t j = 0; j < tamanho; j++) {
            int8_t val = load_matrix(i, j);
            printf("%4d ", val);
        }
        printf("\n");
    }
}

void read_element() {
    uint8_t linha, coluna;
    printf("Linha da matriz (0 a 4):\n");
    scanf("%hhu", &linha);
    getchar();
    printf("Coluna da matriz (0 a 4):\n");
    scanf("%hhu", &coluna);
    getchar();

    int8_t val = load_matrix(linha, coluna);
    printf("Valor lido: %d\n", val);
}

void write_elements() {
    uint8_t matriz_id, linha, coluna;
    int8_t num;
    printf("Id da matriz (0 a 1):\n");
    scanf("%hhu", &matriz_id); getchar();
    printf("Linha da matriz (0 a 4):\n");
    scanf("%hhu", &linha); getchar();
    printf("Coluna da matriz (0 a 4):\n");
    scanf("%hhu", &coluna); getchar();
    printf("Elemento a ser escrito:\n");
    scanf("%hhd", &num); getchar();

    store_matrix(num, linha, coluna, matriz_id);
}

void menu() {
    while (1) {
        int opcao;
        printf("\nEscolha uma opção:\n");
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
        printf("11 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: read_element(); break;
            case 2: write_elements(); break;
            case 3: {
                int8_t num;
                printf("Digite o escalar:\n");
                scanf("%hhd", &num); getchar();
                mult_matrix_esc(num);
                break;
            }
            case 4: add_matrix(); break;
            case 5: sub_matrix(); break;
            case 6: mult_matrix(); break;
            case 7: {
                uint16_t flags = *(uint16_t*)FLAGS_ptr;
                printf("Flags atuais: 0x%04X\n", flags);
                CHECK_FLAGS(flags);
                break;
            }
            case 8: {
                int8_t val = *(int8_t*)DATA_OUT_ptr;
                printf("Saída: %d\n", val);
                break;
            }
            case 9: {
                int num;
                printf("Digite o tamanho da matriz quadrada: ");
                scanf("%d", &num); getchar();
                print_matrix(num);
                break;
            }
            case 10: preencher_matriz_teste(); break;
            case 11:
                running = false;
                printf("Saindo do programa...\n");
                return;
            default:
                printf("Opção inválida, tente novamente.\n");
        }
    }
}

int main() {
    if (init_fpga_mapping() != 0) {
        printf("Erro ao mapear FPGA.\n");
        return -1;
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, monitor_flags, NULL) != 0) {
        perror("pthread_create");
        return -1;
    }

    menu(); // loop principal

    pthread_join(thread_id, NULL);
    return 0;
}
