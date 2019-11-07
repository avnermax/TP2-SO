#include "funcoes.h"

int main(int argc, char *argv[]){
    char e[8], t;
    Memoria *memVirtual;
    IO *io;
    FILE *arq;

    io = ioexec(argv);

    // Abre o arquivo que será testado.
    arq = fopen(io->arq, "r");
    if (arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    // Aloca vetor referente a memória simulada.
    memVirtual = (Memoria*) calloc((io->numPaginas), sizeof(Memoria));

    printf("Executando o simulador...\n");

    // Simula fluxo de dados do arquivo para a memória.
    while(!feof(arq)){
        fscanf(arq, "%s %c\n", e, &t);
        if(t == 'W' || t == 'w'){ // Escreve endereço na memória.
            escreveEndereco(io, memVirtual, e);
        }else if(t == 'R' || t == 'r'){ // Lê endereço da memória.
            if(encontraEndereco(io, memVirtual, e)){
                io->hits++;
            }else{
                io->misses++;
                escreveEndereco(io, memVirtual, e);
            }
            io->leituras++;
        }else{
            printf("Tipo de operação inválida.\n");
            return 0;
        }
        io->operacoes++;
    }

    printf("Arquivo de entrada: %s\n", io->arq);
    printf("Tamanho da memória: %dKB\n", io->tamMemoria);
    printf("Tamanho das páginas: %dKB\n", io->tamPagina);
    printf("Número de páginas: %d\n", io->numPaginas);
    printf("Técnica de reposição: %s\n", io->politicaSubs);
    printf("Quantidade de PageHits: %d\n", io->hits);
    printf("Quantidade de PageMiss: %d\n", io->misses);
    printf("Quantidade de PageFaults: %.1f\n", io->faults / io->escritas * 100);
    printf("Páginas lidas: %d\n", io->leituras);
    printf("Páginas escritas: %d\n", io->escritas);
    printf("Número de Writebacks: %d\n", io->writebacks);

    return 0;
}
