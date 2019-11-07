#include "funcoes.h"

IO * ioexec(char *argv[]){
    IO *io;

    io = (IO*) malloc(sizeof(IO));

    // Reseta valores de controle.
    io->operacoes = 0;
    io->leituras = 0;
    io->escritas = 0;
    io->indice = 0;
    io->hits = 0;
    io->misses = 0;
    io->writebacks = 0;
    io->usedPages = 0;
    io->faults = 0;

    // Grava a política de substituição passado como argumento.
    strcpy(io->politicaSubs, argv[1]);
    if(strcmp(io->politicaSubs, "lru") > 0 || strcmp(io->politicaSubs, "nru") > 0 || strcmp(io->politicaSubs, "segunda_chance") > 0)
        printf("Política de substituição inválida.\n");

    // Grava o arquivo que contém o conjunto de endereços.
    strcpy(io->arq, argv[2]);

    // Grava o tamanho da página passado como argumento.
    io->tamPagina = atoi(argv[3]);
    if(io->tamPagina < 2 || io->tamPagina > 64)
        printf("Tamanho de página inválido.\n");

    // Grava o tamanho da memória passado como argumento.
    io->tamMemoria = atoi(argv[4]);
    if(io->tamMemoria < 128 || io->tamMemoria > 16384)
        printf("Tamanho de memória inválido.\n");

    io->numPaginas = io->tamPagina / io->tamMemoria;

    return io;
}
