#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct T_Memoria{
    int bitR, bitM;
    int contaAcesso;
    float clockacesso;
    char endereco[8];
}Memoria;

typedef struct T_IO{
    /* Variáveis de controle */
    int operacoes, leituras, escritas;
    int hits, misses, writebacks, usedPages;
    float faults;

    /* Dados passados na execução */
    int tamPagina, tamMemoria, numPaginas;
    char politicaSubs[50], arq[50];
}IO;

/* io.c */
IO * ioexec(char *argv[]);

/* funcoes.c */
void LRU(IO *io, Memoria *mem, char *endereco);
void NRU(IO *io, Memoria *mem, char *endereco);
void Segunda_chance(IO *io, Memoria *mem, char *endereco, clock_t t);
void resetaBitR(Memoria *mem, IO *io);
void adicionaEndereco(IO *io, Memoria *mem, char *endereco, clock_t t);
bool encontraEndereco(IO *io, Memoria *mem, char *endereco);
void substituiEndereco(IO *io, Memoria *mem, char *endereco, clock_t t);
void escreveEndereco(IO *io, Memoria *mem, char *endereco, clock_t t);
