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
    memVirtual = (Memoria*) calloc((io->tamMemoria), sizeof(Memoria));

    // Grava dados do arquivo no vetor.
    while(!feof(arq)){
        fscanf(arq, "%s %c\n", e, &t);
        if(t == 'W' || t == 'w'){
            // Escreve endereço na memória.
            escreveEndereco(io, memVirtual, e);
        }else if(t == 'R' || t == 'r'){
            // Lê endereço da memória.
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

    return 0;
}
