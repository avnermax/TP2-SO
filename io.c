#include "tp2virtual.h"

void lerArquivo(char* argv[]){
    char* algoritmo_subsituicao;
    strcpy(algoritmo_subsituicao, argv[1]);
    FILE* arquivo = fopen(argv[2], "r");
    if (arquivo == NULL) printf("ERRO no arquivo");
    int tamanho_pagina = atoi(argv[3]);
    int tamanho_memoria = atoi(argv[4]);

    leitura* enderecos = (leitura*) malloc ((VETOR_ELEMENTOS) * sizeof(leitura));
    int i = 0;
    char tipo;
    int endereco;
    while (!feof(arquivo))
    {
        fscanf(arquivo, "%d %c\n", &endereco, &tipo);
        enderecos[i].endereco = endereco;
        enderecos[i].tipo = tipo;
        printf("%d %c\n", enderecos[i].endereco, enderecos[i].tipo);
        i++;
    }
    free(enderecos);
}