#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct T_Memoria{
    unsigned endereco;
}Memoria;

typedef struct T_Node{
    /* Variáveis de controle */
    int bitR, bitM;
    int contaAcesso;
    float clockacesso;
    unsigned endereco;

    /* Aponta para próximo endereço na hash */
	struct T_Node* prox;
}Node;

typedef struct T_IO{
    /* Variáveis de controle */
    int operacoes, leituras, escritas, hits, misses;
    float faults;

    /* Dados passados na execução */
    unsigned tamPagina, tamMemoria;
    char politicaSubs[50], arq[50];
    int numPaginas;
}IO;

/* io.c */
IO * ioexec(char *argv[]);

/* funcoes.c */
void LRU(IO *io, Node *h, Memoria *mem, unsigned endereco);
void NRU(IO *io, Node *h, Memoria *mem, unsigned endereco);
void Segunda_chance(IO *io, Node *h, Memoria *mem, unsigned endereco, clock_t t);
void resetaBitR(Node *h, IO *io);
unsigned calculaIndice(unsigned endereco, IO *io);
void adicionaEndereco(IO *io, Node *h, Memoria *mem, unsigned indice, clock_t t);
int procuraEnderecoLivre(IO *io, Memoria *mem);
void substituiEndereco(IO *io, Node *h, Memoria *mem, unsigned endereco, clock_t t);
Node * inicializaNode(IO *io);
