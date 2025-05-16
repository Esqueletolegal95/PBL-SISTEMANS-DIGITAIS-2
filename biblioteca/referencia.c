#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "./hps_0.h"

#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define LEDR_BASE 0x0
#define SW_BASE 0x10

int main ( void )
{
    volatile int *LEDR_ptr, *SW_ptr ; // virtual address pointer to red LEDs
    int fd = -1; // used to open / dev / mem
    int count;
    void *LW_virtual ; // physical addresses for light - weight bridge
    // Open /dev/mem to give access to physical addresses
    if (( fd = open ( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1) {
        printf ("ERROR : could not open \"/ dev / mem \"...\n") ;
        return ( -1) ;
    }
    // Get a mapping from physical addresses to virtual addresses
    LW_virtual = mmap ( NULL , LW_BRIDGE_SPAN , ( PROT_READ | PROT_WRITE ) , MAP_SHARED , fd , LW_BRIDGE_BASE ) ;
    if ( LW_virtual == MAP_FAILED ) {
        printf (" ERROR : mmap () failed ...\n") ;
        close ( fd ) ;
        return ( -1) ;
    }

    // Set virtual address pointer to I/O port
    LEDR_ptr  = (int*) ( LW_virtual + LEDR_BASE );
    SW_ptr    = (int*) (LW_virtual + SW_BASE);

    *LEDR_ptr = 0;
    int j = 0; // variável contadora das chaves
    int sw; // variável temporária para fazer a "iteração" pelos bits de *SW_ptr
    while (1) {
        j = 0;
        *LEDR_ptr = 0;
        sw = *SW_ptr;
        for (int i = 0; i < 10; i++) {
            if ((sw) & 1) { // verifica o se o bit menos significativo é 1 e soma j + 1
                j++;
            }
            sw >>= 1; // faz o deslocamento à direita para que todos os bits sejam verificados
        }
        for (int i = 0; i < j; i++) {
            *LEDR_ptr |= 1 << i; // ativa a quantidade de leds correspondente à contagem de switchs ligados
        }
    }

    // Close the previously - opened virtual address mapping
    if ( munmap ( LW_virtual , LW_BRIDGE_SPAN ) != 0) {
        printf (" ERROR : munmap () failed ...\n");
        return ( -1) ;
    }
    // Close /dev /mem to give access to physical addresses
    close ( fd ) ;
    return 0;
}