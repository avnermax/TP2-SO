#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct T_Memoria{
    int bitR, bitM;
    int contaAcesso;
    float clockacesso;
    unsigned endereco;
}Memoria;

typedef struct T_IO{
    /* Variáveis de controle */
    int operacoes, leituras, escritas;
    int hits, misses, writebacks, usedPages;
    float faults;

    /* Dados passados na execução */
    unsigned tamPagina, tamMemoria;
    char politicaSubs[50], arq[50];
    int numPaginas;
}IO;

/* io.c */
IO * ioexec(char *argv[]);

/* funcoes.c */
void LRU(IO *io, Memoria *mem, unsigned endereco);
void NRU(IO *io, Memoria *mem, unsigned endereco);
void Segunda_chance(IO *io, Memoria *mem, unsigned endereco, clock_t t);
void resetaBitR(Memoria *mem, IO *io);
unsigned calculaIndice(unsigned endereco, unsigned tamPagina);
void adicionaEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t);
int encontraEndereco(IO *io, Memoria *mem, unsigned endereco);
void substituiEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t);
void escreveEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t);
