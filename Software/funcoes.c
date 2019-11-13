#include "funcoes.h"

/* POLITICAS DE SUBSTITUICAO */
void LRU(IO *io, Node *h, Memoria *mem, unsigned endereco){
    int i, menor, c = 0;

    // Encontra o indice do endereco com menor numero de acesso.
    menor = h[0].contaAcesso;
    for(i = 1; i < io->numPaginas; i++){
        if(h[i].contaAcesso < menor){
            menor = h[i].contaAcesso;
            c = i; // Recebe o indice do endereco com menor numero de acesso.
        }
    }

    h[c].endereco = endereco;
    h[c].contaAcesso++; // Conta acesso do endereco recem copiado.
}

void NRU(IO *io, Node *h, Memoria *mem, unsigned endereco){
    for (int i = 0; i < io->numPaginas; i++){
        if (h[i].bitR == 0 && h[i].bitM == 0){  //classe 00: nao referenciada, nao modificada
            h[i].endereco = endereco;
            h[i].bitM = 1;
            h[i].bitR = 1;
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (h[i].bitR == 0 && h[i].bitM == 1){  //classe 01: nao referenciada, modificada
            h[i].endereco = endereco;
            h[i].bitR = 1;
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (h[i].bitR == 1 && h[i].bitM == 0){   //classe 10: referenciada, nao modificada
            h[i].endereco = endereco;
            h[i].bitM = 1;
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (h[i].bitR == 1 && h[i].bitM == 1){   //classe 11: referenciada, modificada
            h[i].endereco = endereco;
            return;
        }
    }
}

void Segunda_chance(IO *io, Node *h, Memoria *mem, unsigned endereco, clock_t t){
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
        h[c].endereco = endereco;
        h[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        h[c].bitR = 1;
        h[c].contaAcesso++; // Conta acesso do endereco recem copiado.
    }else{
        // Nova chance a mem[c]
        h[c].bitR = 0;
        h[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        Segunda_chance(io, h, mem, endereco, t);
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

void adicionaEndereco(IO *io, Node *h, Memoria *mem, unsigned indice, clock_t t){
	/* Necessario conferir se não faltou nada */
	int endF, ad;
	Node *temp, *anterior;
	if(h[indice].prox == NULL){
        // Marca falta de página.
        io->faults++;

		h[indice].endereco = indice;
		h[indice].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
		h[indice].bitR = 0;
    	h[indice].bitM = 1;
   		h[indice].contaAcesso++;
 	}else{
		temp = h[indice].prox;
		while(temp != NULL){
			if (temp->endereco == indice){
				ad = 1;
				break;
			}else{
				ad = 0;
				anterior = temp;
				temp = temp->prox;
			}
		}
		if(ad == 0){
            // Marca falta de página
    		io->faults++;

			endF = procuraEnderecoLivre(io, mem);;
			Node *aux = (Node*) malloc(sizeof(Node));
			mem[endF].endereco = indice;
			aux->prox = NULL;
			anterior->prox = aux;
			aux->endereco = indice;
			aux->clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
			aux->bitR = 0;
    		aux->bitM = 1;
    		aux->contaAcesso++; // Conta acesso do endereco recem copiado.
		}
	}
	io->operacoes++;
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

void substituiEndereco(IO *io, Node *h, Memoria *mem, unsigned endereco, clock_t t){
	if(strcmp(io->politicaSubs, "lru") == 0){
		LRU(io, h, mem, endereco);
	}else if(strcmp(io->politicaSubs, "nru") == 0){
		NRU(io, h, mem, endereco);
	}else if(strcmp(io->politicaSubs, "segunda_chance") == 0){
		Segunda_chance(io, h, mem, endereco, t);
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
        tNode[i].endereco = -1;
    }

    return tNode;
}
