#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VETOR_ELEMENTOS 1000000

/*
endereco = endereço de memoria
tipo = "W" pra escrita, R pra leitura
*/
typedef struct adress{
    int endereco;
    char tipo;
} leitura;

void lerArquivo(char* argv[]);