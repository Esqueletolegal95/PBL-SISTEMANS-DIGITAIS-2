# Nome do executável final
TARGET = main

# Compilador e flags
CC = gcc
CFLAGS = -Wall -O2 -march=armv7-a -std=c99 -O0
LDFLAGS = -lpthread            # Adicionado para suportar pthread
AS = as
ASFLAGS = -march=armv7-a

# Fontes
SRC_C = main.c
SRC_S = coprocessor.s
OBJS = main.o coprocessor.o

# Diretórios
INCLUDES = -I.

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.c coprocessor.h hps_0.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

coprocessor.o: coprocessor.s
	$(AS) $(ASFLAGS) -o $@ $<

# Limpeza
clean:
	rm -f *.o $(TARGET)
