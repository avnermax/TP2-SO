#include "funcoes.h"

/* POLITICAS DE SUBSTITUICAO */
void LRU(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina){
    Node *aux, *menor;

    aux = (Node*) malloc(sizeof(Node));
    menor = (Node*) malloc(sizeof(Node));

    io->hits++;
    aux = h[indice].prox; // Armazena em 'aux' o proximo NO do NO cabeça, tendo o NO cabeça dado pelo 'indice';
    menor = aux;
    while(aux != NULL){
        if(aux->contaAcesso < menor->contaAcesso){
            menor = aux; // Recebe o NO com menor valor de numero de acesso.
        }
        aux = aux->prox;
    }

    menor->pagina = pagina;
    menor->contaAcesso++; // Conta acesso do endereco recem copiado.
}

void NRU(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina){
    int menorTemp[2];
    Node *menor, *aux;

    aux = (Node*) malloc(sizeof(Node)); // Armazena node para caminhar na lista encadeada.
    menor = (Node*) malloc(sizeof(Node)); // Armazena node de menor classe.

    io->hits++;
    aux = h[indice].prox; // Armazena em 'aux' o proximo NO do NO cabeça, tendo o NO cabeça dado pelo 'indice';
    menorTemp[0] = h[indice].bitR;
    menorTemp[1] = h[indice].bitM;
    while(aux != NULL){ // Procura entre todos da lista encadeada, o de menor classe.
        if(aux->bitR == 0 && aux->bitM == 0){
            aux->pagina = pagina;
            aux->bitM = 1;
            aux->bitR = 1;
            menor = aux;
            break;
        }else if(aux->bitR < menorTemp[0]){
            menorTemp[0] = aux->bitR;
            menorTemp[1] = aux->bitM;
            menor = aux;
        }else if(aux->bitR == menorTemp[0]){
            if(aux->bitM < menorTemp[1]){
                menorTemp[1] = aux->bitM;
                menor = aux;
            }
        }else{
            menor = aux;
        }
        aux = aux->prox;
    }

    menor->pagina = pagina;
    menor->contaAcesso++; // Conta acesso do endereco recem copiado.

    /* Não sei se está gravando na hash o valor do endereço, a partir do 'menor'...
    depois me ajuda a testar isso. (mas ao meu ver, nem precisa. Por mim deixa do jeito que tá e sucesso. kkkkkk) */
}

void Segunda_chance(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina, clock_t t){
    Node *menorClock, *aux;

    menorClock = (Node*) malloc(sizeof(Node));
    aux = (Node*) malloc(sizeof(Node));

    io->hits++;
    menorClock = h[indice].prox; // Armazena em 'menorClock' o proximo NO do NO cabeça, tendo o NO cabeça dado pelo 'indice';
    aux = h[indice].prox;

    if(menorClock->bitR == 0){
        menorClock->pagina = pagina;
        menorClock->bitR = 1;
        menorClock->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        menorClock->contaAcesso++; // Conta acesso do endereco recem copiado.
    }else{
        menorClock->bitR = 0;
        menorClock->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        while(aux != NULL) aux = aux->prox; // Caminha até o NULL.

        // Troca as apontadores para dar uma nova chance ao endereco com menor clock.
        aux = h[indice].prox;
        h[indice].prox = menorClock->prox;
        menorClock->prox = NULL;
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

void adicionaEndereco(IO *io, Node *hash, Memoria *mem, unsigned indice, unsigned pagina, clock_t t){
	int endF, ad = 0;
	Node *temp, *anterior;

	if(hash[indice].prox == NULL){
        // Marca falta de página.
        io->faults++;

        temp = (Node*) malloc(sizeof(Node));

        endF = procuraEnderecoLivre(io, mem);
        mem[endF].endereco = pagina;

        temp->prox = NULL;
        temp->pagina = pagina;
        temp->endFisico = endF;
        temp->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        temp->bitR = 0;
        temp->bitM = 1;
        temp->contaAcesso++; // Conta acesso do endereco recem copiado.

        hash[indice].prox = temp;
 	}else{
		temp = hash[indice].prox;

		while(temp != NULL){
			if(temp->pagina == pagina){
				ad = 1;
                io->hits++;
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

void substituiEndereco(IO *io, Node *h, Memoria *mem, unsigned indice, unsigned pagina, clock_t t){
    if(strcmp(io->politicaSubs, "lru") == 0){
        LRU(io, h, mem, indice, pagina);
	}else if(strcmp(io->politicaSubs, "nru") == 0){
		NRU(io, h, mem, indice, pagina);
	}else if(strcmp(io->politicaSubs, "segunda_chance") == 0){
        Segunda_chance(io, h, mem, indice, pagina, t);
	}
}

Node * inicializaNode(IO *io){
    Node *tNode;
    int i;

    tNode = (Node*) malloc(io->numPaginas * sizeof(Node));

    // Reseta todos os NOS cabeças.
    for(i = 0; i < io->numPaginas; i++){
        tNode[i].prox = NULL;
        tNode[i].bitR = 0;
        tNode[i].bitM = 0;
        tNode[i].contaAcesso = 0;
        tNode[i].clockacesso = 0;
        tNode[i].pagina = 0;
    }

    return tNode;
}
