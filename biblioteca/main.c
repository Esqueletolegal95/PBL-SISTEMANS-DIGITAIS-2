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



uint8_t read_element(){
    uint8_t linha, coluna;
    printf("Linha da matriz (0 a 4)\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4)\n");
    scanf("%hhu", &coluna);
    return load_matrix(linha, coluna);
}

void write_elements(){
    uint8_t matriz_id, linha, coluna;
    int8_t num;
    printf("Id da matrix (0 a 1)\n");
    scanf("%hhu", &matriz_id);
    printf("Linha da matriz (0 a 4)\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4)\n");
    scanf("%hhu", &coluna);
    printf("Primeiro elemento:\n");
    scanf("%hhd", &num);
    *WR_ptr = 1;
    store_matrix(num, linha, coluna, matriz_id);
    *WR_ptr = 0;
}


void menu(){
    while (1)
    {
        int opcao;
        printf("Escolha uma opcao:\n");
        printf("1 - Ler elemento na matriz\n");
        printf("2 - escrever elementos na matriz\n");
        printf("3 - multiplicar matriz por um escalar\n");
        printf("4 - somar matrizes\n");
        printf("5 - subtrair matrizes\n");
        printf("6 - multiplicar matrizes\n");
        printf("7 - mostrar flags" );
        printf("8 - mostrar saída \n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            uint8_t linha, coluna;
            printf("Linha da matriz (0 a 4)\n");
            scanf("%hhu", &linha);
            printf("Coluna da matriz (0 a 4)\n");
            scanf("%hhu", &coluna);
            load_matrix(linha, coluna);
            printf("Operação: Leitura de elemento realizada.\n");
            break;
        case 2:
            uint8_t matriz_id, linha, coluna, id;
            printf("Id da matrix (0 ou 1)\n");
            scanf("%hhu", &matriz_id);
            printf("Linha da matriz (0 a 4)\n");
            scanf("%hhu", &linha);
            printf("Coluna da matriz (0 a 4)\n");
            scanf("%hhu", &coluna);
            write_elements();
            printf("Operação: Escrita de elementos realizada.\n");
            break;
        case 3: {
            int8_t num;
            printf("Digite o escalar:\n");
            scanf("%hhd", &num);
            mult_matrix_esc(num);
            printf("Operação: Multiplicação por escalar realizada.\n");
            break;
        }
        case 4:
            sum_matrix();
            printf("Operação: Soma de matrizes realizada.\n");
            break;
        case 5:
            sub_matrix();
            printf("Operação: Subtração de matrizes realizada.\n");
            break;
        case 6:
            mult_matrix();
            printf("Operação: Multiplicação de matrizes realizada.\n");
            break;
        case 7:
            printf("%u\n", *(uint16_t*)FLAGS_ptr);
            break;
        case 8:
            printf("%u\n", *(int8_t*)DATA_OUT_ptr);
            break;
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
