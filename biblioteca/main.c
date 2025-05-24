#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include "coprocessor.h"
#include "./hps_0.h"

#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x1000
#define MATRIX_SIZE 5

volatile uint32_t *INSTRUCTION_ptr;
volatile uint32_t *FLAGS_ptr;
volatile uint32_t *WR_ptr;
volatile uint32_t *DATA_OUT_ptr;

pthread_t flag_thread;
uint16_t last_flags = 0;

void show_flags_terminal(uint16_t f) {
    printf("\033[s");                    // Salva cursor
    printf("\033[999;0H");               // Vai pra linha de baixo
    printf("\033[2K");                   // Limpa a linha
    printf("FLAGS: 0x%04X  ", f);
    if (f & FLAG_DONE) printf("✅ ");
    if (f & FLAG_OVERFLOW) printf("⚠️ ");
    if (f & FLAG_INCORRECT_ADDR) printf("❌ ");
    printf("\033[u");                    // Restaura cursor
    fflush(stdout);
}

void *flag_watcher(void *arg) {
    while (1) {
        uint16_t f = *(volatile uint16_t*)FLAGS_ptr;
        if (f != last_flags) {
            last_flags = f;
            show_flags_terminal(f);
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
    for (uint8_t linha = 0; linha < MATRIX_SIZE; linha++) {
        for (uint8_t coluna = 0; coluna < MATRIX_SIZE; coluna++) {
            int8_t valor = linha * MATRIX_SIZE + coluna;
            store_matrix(valor, linha, coluna, 0);
        }
    }
    printf("✔️ Matriz preenchida com sucesso!\n");
}

void print_matrix(int tamanho) {
    printf("Conteúdo da matriz 0:\n");
    for (uint8_t i = 0; i < tamanho; i++) {
        for (uint8_t j = 0; j < tamanho; j++) {
            int8_t valor = load_matrix(i, j);
            printf("%4d ", valor);
        }
        printf("\n");
    }
}

int8_t read_element() {
    uint8_t linha, coluna;
    printf("Linha da matriz (0 a 4): ");
    scanf("%hhu", &linha); getchar();
    printf("Coluna da matriz (0 a 4): ");
    scanf("%hhu", &coluna); getchar();

    int8_t valor = load_matrix(linha, coluna);
    printf("Valor lido: %d\n", valor);
    return valor;
}

void write_element() {
    uint8_t matriz_id, linha, coluna;
    int8_t num;
    printf("Id da matriz (0 a 1): ");
    scanf("%hhu", &matriz_id); getchar();
    printf("Linha da matriz (0 a 4): ");
    scanf("%hhu", &linha); getchar();
    printf("Coluna da matriz (0 a 4): ");
    scanf("%hhu", &coluna); getchar();
    printf("Elemento a ser escrito: ");
    scanf("%hhd", &num); getchar();

    store_matrix(num, linha, coluna, matriz_id);
}

void menu() {
    while (1) {
        system("clear");
        printf("=== 🧠 Menu Principal - Coprocessador de Matrizes ===\n");
        printf("1 - Ler elemento da matriz\n");
        printf("2 - Escrever elemento na matriz\n");
        printf("3 - Multiplicar por escalar\n");
        printf("4 - Somar matrizes\n");
        printf("5 - Subtrair matrizes\n");
        printf("6 - Multiplicar matrizes\n");
        printf("7 - Ver valor de saída\n");
        printf("8 - Imprimir matriz\n");
        printf("9 - Preencher matriz de teste\n");
        printf("10 - Resetar matrizes (via coprocessador)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");

        int opcao;
        scanf("%d", &opcao); getchar();
        system("clear");

        switch (opcao) {
            case 1:
                read_element();
                break;
            case 2:
                write_element();
                break;
            case 3: {
                int8_t num;
                printf("Digite o escalar: ");
                scanf("%hhd", &num); getchar();
                mult_matrix_esc(num);
                break;
            }
            case 4:
                add_matrix();
                break;
            case 5:
                sub_matrix();
                break;
            case 6:
                mult_matrix();
                break;
            case 7: {
                int8_t val = *(volatile int8_t*)DATA_OUT_ptr;
                printf("Saída: %d\n", val);
                break;
            }
            case 8: {
                int size;
                printf("Tamanho da matriz: ");
                scanf("%d", &size); getchar();
                print_matrix(size);
                break;
            }
            case 9:
                preencher_matriz_teste();
                break;
            case 11: {
                reset_matrix();
                break;
                }

            case 0:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opção inválida!\n");
                break;
        }

        printf("\nPressione Enter para continuar...");
        getchar();
    }
}

int main() {
    if (init_fpga_mapping() != 0) {
        printf("Erro ao mapear FPGA.\n");
        return -1;
    }

    if (pthread_create(&flag_thread, NULL, flag_watcher, NULL)) {
        printf("Erro ao criar thread de flags!\n");
        return -1;
    }

    menu();
    return 0;
}
