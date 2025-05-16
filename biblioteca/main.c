#include <stdio.h>
#include "biblioteca.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
//#include "./hps_0.h"

#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x1000
extern volatile uint32_t *FPGA_ADSS;


int init_fpga_mapping() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    void *mapped_base = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (mapped_base == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    FPGA_ADSS = (volatile uint32_t *)mapped_base;
    close(fd);
    return 0;
}



uint8_t make_id(uint8_t matriz, uint8_t linha, uint8_t coluna) {
    return ((matriz & 0x03) << 6) | ((linha & 0x07) << 3) | (coluna & 0x07);
}

uint8_t read_element(){
    uint8_t matriz_id, linha, coluna, id;
    printf("Id da matrix (0 a 2)\n");
    scanf("%hhu", &matriz_id);
    printf("Linha da matriz (0 a 4)\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4)\n");
    scanf("%hhu", &coluna);
    id = make_id(matriz_id, linha, coluna);
    return read_matrix(id);
}

void write_elements(){
    uint8_t matriz_id, linha, coluna, id;
    int8_t num1, num2;
    printf("Id da matrix (0 a 2)\n");
    scanf("%hhu", &matriz_id);
    printf("Linha da matriz (0 a 4)\n");
    scanf("%hhu", &linha);
    printf("Coluna da matriz (0 a 4)\n");
    scanf("%hhu", &coluna);
    id = make_id(matriz_id, linha, coluna);
    printf("Primeiro elemento:\n");
    scanf("%hhd", &num1);
    printf("Segundo elemento:\n");
    scanf("%hhd", &num2);
    write_matrix(num1, num2, id);
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
        printf("7 - matriz transposta\n");
        printf("8 - matriz oposta\n");
        printf("9 - determinante 2x2\n");
        printf("10 - determinante 3x3\n");
        printf("11 - determinante 4x4\n");
        printf("12 - determinante 5x5\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            read_element();
            printf("Operação: Leitura de elemento realizada.\n");
            break;
        case 2:
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
            trans_matrix();
            printf("Operação: Transposição de matriz realizada.\n");
            break;
        case 8:
            oppo_matrix();
            printf("Operação: Matriz oposta calculada.\n");
            break;
        case 9:
            det2_matrix();
            printf("Operação: Determinante 2x2 calculado.\n");
            break;
        case 10:
            det3_matrix();
            printf("Operação: Determinante 3x3 calculado.\n");
            break;
        case 11:
            det4_matrix();
            printf("Operação: Determinante 4x4 calculado.\n");
            break;
        case 12:
            det5_matrix();
            printf("Operação: Determinante 5x5 calculado.\n");
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
