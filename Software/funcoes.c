#include "funcoes.h"

/* POLITICAS DE SUBSTITUICAO */
void LRU(IO *io, Node *h, Memoria *mem, unsigned pagina){
    int i, menor, c = 0;

    // Encontra o indice do endereco com menor numero de acesso.
    for(i = 0; i < io->numPaginas; i++){
        if(mem[i].endereco == pagina){ // Substitui na página específica.
            io->hits++;
            aux = h[i].prox;
            menor = h[i].contaAcesso;
            while(aux->prox!=NULL){
                if(aux->contaAcesso < menor){
                    menor = h[i].contaAcesso; // Recebe o indice do endereco com menor numero de acesso.
                }
            }
        }
    }

    h[c].pagina = pagina;
    h[c].contaAcesso++; // Conta acesso do endereco recem copiado.
}

void NRU(IO *io, Node *h, Memoria *mem, unsigned pagina){
    int menorTemp[2], cont, i;
    Node *temp, *aux;

    aux = (Node*) malloc(sizeof(Node)); // Armazena node para caminhar na lista encadeada.
    temp = (Node*) malloc(sizeof(Node)); // Armazena node de menor classe.

    cont = 0;
    for(i = 0; i < io->numPaginas; i++){
        if(mem[i].endereco == pagina){ // Substitui na página específica.
            menorTemp[0] = h[i].bitR;
            menorTemp[1] = h[i].bitM;
            io->hits++;
            do{ // Procura entre todos da lista encadeada.
                printf("Aquui\n");
                aux = h[i].prox;
                printf("%d %d\n",aux->bitR,aux->bitM);
                if (aux->bitR == 0 && aux->bitM == 0){
                    printf("aquui 00\n");
                    aux->pagina = pagina;
                    aux->bitM = 1;
                    aux->bitR = 1;
                    aux = h[i].prox;
                    return;
                }else if(aux->bitR < menorTemp[0]){
                    printf("aquui 0X\n");
                    menorTemp[0] = aux->bitR;
                    menorTemp[1] = aux->bitM;
                    temp = aux;
                    cont = i;
                }else if(aux->bitR == menorTemp[0]){
                    if(aux->bitM < menorTemp[1]){
                        printf("aquui X0\n");
                        menorTemp[1] = aux->bitM;
                        temp = aux;
                        cont = i;
                    }
                }else{
                    temp = aux;
                }
            }while(aux->prox != NULL);
        }
    }

    h[cont].prox = temp->prox;
    h[cont].pagina = temp->pagina;
    h[cont].bitR = temp->bitR;
    h[cont].bitM = temp->bitM;
    h[cont].clockacesso = temp->clockacesso;
    h[cont].contaAcesso = temp->contaAcesso;
}

void Segunda_chance(IO *io, Node *h, Memoria *mem, unsigned pagina, clock_t t){
    int i, c = 0;
    float menorClock;

    // Encontra o indice do endereco com o acesso mais antigo.
    menorClock = h[0].clockacesso;
    for(i = 1; i < io->numPaginas; i++){
        if(h[i].clockacesso < menorClock){
            menorClock = h[i].clockacesso;
            c = i; // Recebe o indice do menor clock encontrado.
        }
    }

    if(h[c].bitR == 0){
        // Substitue mem[c]
        h[c].pagina = pagina;
        h[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        h[c].bitR = 1;
        h[c].contaAcesso++; // Conta acesso do endereco recem copiado.
    }else{
        // Nova chance a mem[c]
        h[c].bitR = 0;
        h[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        Segunda_chance(io, h, mem, pagina, t);
    }
}

/* FUNCOES */
void resetaBitR(Node *h, IO *io){
    int i;
    for(i = 0; i < io->numPaginas; i++){
        h[i].bitR = 0;
    }
}

unsigned calculaIndice(unsigned endereco, IO *io){
    unsigned s, page, tmp;

    s = 0;
    tmp = io->tamPagina;
    while(tmp > 1){
        tmp = tmp >> 1;
        s++;
    }

    page = endereco >> s;

    return page;
}

void adicionaEndereco(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina, clock_t t){
	/* Necessario conferir se não faltou nada */
	int endF, ad = 0;
	Node *temp, *anterior;

	if(h[indice].prox == NULL){
        // Marca falta de página.
        io->faults++;

        temp = (Node*) malloc(sizeof(Node));

        endF = procuraEnderecoLivre(io, mem);
        mem[endF].endereco = pagina;
        printf("1\n");
        printf("endF:%d\n", endF);
        printf("mem:%d\n", mem[endF].endereco);

        temp->prox = NULL;
        temp->pagina = pagina;
        temp->endFisico = endF;
        temp->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        temp->bitR = 0;
        temp->bitM = 1;
        temp->contaAcesso++; // Conta acesso do endereco recem copiado.

        h[indice].prox = temp;
 	}else{
		temp = h[indice].prox;

		while(temp != NULL){
			if(temp->pagina == pagina){
				ad = 1;
                //io->hits++;
                //printf("\n\nHIT pagina:%d\n\n", pagina);
				break;
			}else{
				anterior = temp;
				temp = temp->prox;
			}
		}

        if(ad == 0){
            // Marca falta de página
            io->faults++;

            endF = procuraEnderecoLivre(io, mem);
            mem[endF].endereco = pagina;
            printf("2\n");
            printf("endF:%d\n", endF);
            printf("mem:%d\n", mem[endF].endereco);

            Node *aux = (Node*) malloc(sizeof(Node));
            aux->prox = NULL;
            aux->pagina = pagina;
            aux->endFisico = endF;
            aux->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
            aux->bitR = 0;
            aux->bitM = 1;
            aux->contaAcesso++; // Conta acesso do endereco recem copiado.

            anterior->prox = aux;
        }
    }
}

int procuraEnderecoLivre(IO *io, Memoria *mem){
    int i;

    for(i = 0; i < io->numPaginas; i++){
		if(mem[i].endereco == 0){
			return i;
		}
	}

    // Retorna '-1' caso esteja cheia;
	return -1;
}

void substituiEndereco(IO *io, Node *h, Memoria *mem, unsigned pagina, clock_t t){
    if(strcmp(io->politicaSubs, "lru") == 0){
        LRU(io, h, mem, pagina);
	}else if(strcmp(io->politicaSubs, "nru") == 0){
		NRU(io, h, mem, pagina);
	}else if(strcmp(io->politicaSubs, "segunda_chance") == 0){
        Segunda_chance(io, h, mem, pagina, t);
	}
}

Node * inicializaNode(IO *io){
    Node *tNode;
    int i;

    tNode = (Node*) malloc(io->numPaginas * sizeof(Node));

    for(i = 0; i < io->numPaginas; i++){
        tNode[i].prox = NULL;
        tNode[i].bitR = 0;
        tNode[i].bitM = 0;
        tNode[i].contaAcesso = 0;
        tNode[i].clockacesso = 0;
        tNode[i].pagina = -1;
    }

    return tNode;
}
