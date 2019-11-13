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
    int bitR, bitM, endFisico;
    int contaAcesso;
    float clockacesso;
    unsigned pagina;

    /* Aponta para próximo endereço na hash */
	struct T_Node *prox;
}Node;

typedef struct T_IO{
    /* Variáveis de controle */
    int operacoes, leituras, escritas, hits, faults;

    /* Dados passados na execução */
    unsigned tamPagina, tamMemoria;
    char politicaSubs[50], arq[50];
    int numPaginas;
}IO;

/* io.c */
IO * ioexec(char *argv[]);

/* funcoes.c */
void LRU(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina);
void NRU(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina);
void Segunda_chance(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina, clock_t t);
void resetaBitR(Node *h, IO *io);
unsigned calculaIndice(unsigned endereco, IO *io);
void adicionaEndereco(IO *io, Node *hash, Memoria *mem, unsigned indice, unsigned pagina, clock_t t);
int procuraEnderecoLivre(IO *io, Memoria *mem);
void substituiEndereco(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina, clock_t t);
Node * inicializaNode(IO *io);
