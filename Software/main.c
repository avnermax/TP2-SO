#include "funcoes.h"

int main(int argc, char *argv[]){
    char e[8], t;
    Memoria *memVirtual;
    IO *io;
    FILE *arq;
    clock_t tempo, tAtual;

    io = ioexec(argv);

    // Abre o arquivo que sera testado.
    arq = fopen(io->arq, "r");
    if (arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    // Aloca vetor referente a memoria simulada.
    memVirtual = (Memoria*) calloc((io->numPaginas), sizeof(Memoria));

    printf("Executando o simulador...\n");

    // Recebe como parametro as horas do sistema.
	tempo = clock();

    // Simula fluxo de dados do arquivo para a memoria.
    while(!feof(arq)){
        fscanf(arq, "%s %c\n", e, &t);
        if(t == 'W'){ // Escreve endereço na memoria.
            escreveEndereco(io, memVirtual, e, tempo);
        }else if(t == 'R'){ // Le endereço da memoria.
            if(encontraEndereco(io, memVirtual, e)){
                io->hits++;
            }else{
                io->misses++;
                escreveEndereco(io, memVirtual, e, tempo);
            }
            io->leituras++;
        }else{
            printf("Tipo de operação invalida.\n");
            return 0;
        }

        // Reseta o bitR dos enderecos.
        tAtual = clock();
        if((double)(tAtual - tempo) / CLOCKS_PER_SEC >= 0.1){ // A cada 100ms.
            resetaBitR(memVirtual, io);
            tempo = clock();
        }

        io->operacoes++;
    }

    printf("Arquivo de entrada: %s\n", io->arq);
    printf("Tamanho da memoria: %dKB\n", io->tamMemoria);
    printf("Tamanho das paginas: %dKB\n", io->tamPagina);
    printf("Numero de paginas: %d\n", io->numPaginas);
    printf("Tecnica de substituicao: %s\n", io->politicaSubs);
    printf("Quantidade de PageHits: %d\n", io->hits);
    printf("Quantidade de PageMiss: %d\n", io->misses);
    printf("Quantidade de PageFaults: %.1f\n", io->faults / io->escritas * 100);
    printf("Paginas lidas: %d\n", io->leituras);
    printf("Paginas escritas: %d\n", io->escritas);
    printf("Numero de Writebacks: %d\n", io->writebacks);
    printf("Numero de operacoes executadas: %d\n", io->operacoes);

    return 0;
}
