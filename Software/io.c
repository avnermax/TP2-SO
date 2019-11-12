#include "funcoes.h"

IO * ioexec(char *argv[]){
    IO *io;

    io = (IO*) malloc(sizeof(IO));

    // Reseta valores de controle.
    io->operacoes = 0;
    io->leituras = 0;
    io->escritas = 0;
    io->hits = 0;
    io->misses = 0;
    io->faults = 0;

    // Grava a política de substituicao passado como argumento.
    strcpy(io->politicaSubs, argv[1]);
    if(strcmp(io->politicaSubs, "lru") > 0 && strcmp(io->politicaSubs, "nru") > 0 && strcmp(io->politicaSubs, "segunda_chance") > 0){
        printf("Politica de substituicao invalida.\n");
        exit(EXIT_SUCCESS);
    }

    // Grava o arquivo que contem o conjunto de enderecos.
    strcpy(io->arq, argv[2]);

    // Grava o tamanho da página passado como argumento.
    io->tamPagina = atoi(argv[3]);
    if(io->tamPagina < 2 && io->tamPagina > 64){
        printf("Tamanho de pagina %dKB, invalido.\n", io->tamPagina);
        exit(EXIT_SUCCESS);
    }else{
        io->tamPagina = io->tamPagina * 1024;
    }

    // Grava o tamanho da memoria passado como argumento.
    io->tamMemoria = atoi(argv[4]);
    if(io->tamMemoria < 128 && io->tamMemoria > 16384){
        printf("Tamanho de memoria %dKB, invalido.\n", io->tamMemoria);
        exit(EXIT_SUCCESS);
    }else{
        io->tamMemoria = io->tamMemoria * 1024;
    }

    io->numPaginas = io->tamMemoria / io->tamPagina;

    return io;
}
