#include "funcoes.h"

int main(int argc, char *argv[]){
    unsigned e, page, indiceHash;
    Memoria *mem;
    Node *tabelaHash;
    char t;
    int i, s;
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

    // Inicializa tabela de hash que será a tabela invertida de endereços.
    tabelaHash = inicializaNode(io);

    // Cria memória física zerada.
    mem = (Memoria*) calloc(io->numPaginas, sizeof(Memoria));
    for(i = 0; i < io->numPaginas; i++){
        mem[i].endereco = -1;
    }

    printf("Executando o simulador...\n");

    // Grava a hora do sistema para calculos posteriores.
	tempo = clock();

    // Simula fluxo de dados do arquivo para a memoria.
    while(!feof(arq)){
        fscanf(arq, "%x %c\n", &e, &t);

        page = calculaIndice(e, io);
        indiceHash = page % io->numPaginas;

        s = procuraEnderecoLivre(io, mem);
        if(s != -1){
            // Se achar espaço livre na memoria fisica, adiciona o endereco.
            adicionaEndereco(io, tabelaHash, mem, indiceHash, tempo);
        }else{
            printf("BLE\n");
            // Se nao achar espaço livre, faz a politica de substituicao.
            substituiEndereco(io, tabelaHash, mem, indiceHash, tempo);
        }

        if(t == 'W'){ // Escreve endereço na memoria.
            io->escritas++;
        }else if(t == 'R'){ // Le endereço da memoria.
            io->leituras++;
        }else{
            printf("Tipo de operação invalida.\n");
            exit(EXIT_SUCCESS);
        }

        // Reseta o bitR dos enderecos.
        tAtual = clock();
        if((double)(tAtual - tempo) / CLOCKS_PER_SEC >= 0.1){ // A cada 100ms.
            resetaBitR(tabelaHash, io);
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
    printf("Quantidade de PageFaults: %.1f\n", io->faults);
    printf("Paginas lidas: %d\n", io->leituras);
    printf("Paginas escritas: %d\n", io->escritas);
    printf("Numero de operacoes executadas: %d\n", io->operacoes);

    // Fecha arquivo e libera memórias alocadas.
    fclose(arq);
    //free(io);
    //free(tabelaHash);

    return 0;
}
