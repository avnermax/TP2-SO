#include "funcoes.h"

/* POLITICAS DE SUBSTITUICAO */
void LRU(IO *io, Memoria *mem, unsigned endereco){
    int i, menor, c = 0;

    // Encontra o indice do endereco com menor numero de acesso.
    menor = mem[0].contaAcesso;
    for(i = 1; i < io->numPaginas; i++){
        if(mem[i].contaAcesso < menor){
            menor = mem[i].contaAcesso;
            c = i; // Recebe o indice do endereco com menor numero de acesso.
        }
    }

    mem[c].endereco = endereco;
    mem[c].contaAcesso++; // Conta acesso do endereco recem copiado.
}

void NRU(IO *io, Memoria *mem, unsigned endereco){
    for (int i = 0; i < io->numPaginas; i++){
        if (mem[i].bitR == 0 && mem[i].bitM == 0){  //classe 00: nao referenciada, nao modificada 
            strcpy(mem[i].endereco, endereco);
            mem[i].bitM = 1;
            mem[i].bitR = 1; 
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (mem[i].bitR == 0 && mem[i].bitM == 1){  //classe 01: nao referenciada, modificada
            strcpy(mem[i].endereco, endereco);
            mem[i].bitR = 1;
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (mem[i].bitR == 1 && mem[i].bitM == 0){   //classe 10: referenciada, nao modificada
            strcpy(mem[i].endereco, endereco);
            mem[i].bitM = 1;
            return;
        }
    }

    for (int i = 0; i < io->numPaginas; i++){
        if (mem[i].bitR == 1 && mem[i].bitM == 1){   //classe 11: referenciada, modificada
            strcpy(mem[i].endereco, endereco);
            return;
        }
    }
}

void Segunda_chance(IO *io, Memoria *mem, unsigned endereco, clock_t t){
    int i, c = 0;
    float menorClock;

    // Encontra o indice do endereco com o acesso mais antigo.
    menorClock = mem[0].clockacesso;
    for(i = 1; i < io->numPaginas; i++){
        if(mem[i].clockacesso < menorClock){
            menorClock = mem[i].clockacesso;
            c = i; // Recebe o indice do menor clock encontrado.
        }
    }

    if(mem[c].bitR == 0){
        // Substitue mem[c]
        mem[c].endereco = endereco;
        mem[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        mem[c].bitR = 1;
        mem[c].contaAcesso++; // Conta acesso do endereco recem copiado.
    }else{
        // Nova chance a mem[c]
        mem[c].bitR = 0;
        mem[c].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
        Segunda_chance(io, mem, endereco, t);
    }
}

/* FUNCOES */
void resetaBitR(Memoria *mem, IO *io){
    int i;
    for(i = 0; i < io->numPaginas; i++){
        mem[i].bitR = 0;
    }
}

unsigned calculaIndice(unsigned endereco, unsigned tamPagina){
    unsigned s, page, tmp;

    s = 0;
    tmp = tamPagina;
    while(tmp > 1){
        tmp = tmp >> 1;
        s++;
    }

    page = endereco >> s;
    printf("s:%x\n", s);
    printf("endereco:%d page:%d\n", endereco, page);
    return page;
}

void adicionaEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t){
    unsigned indice;

    if(mem[io->usedPages].bitR == 0 && mem[io->usedPages].bitM == 0){ // Classe 0 NRU.
        indice = calculaIndice(endereco, io->tamPagina);
        printf("indice:%x\n", indice);

        mem[indice].endereco = endereco;
    	mem[indice].clockacesso = (double)(clock() - t) / CLOCKS_PER_SEC;
    	mem[indice].bitR = 1;
        mem[indice].bitM = 1;
        mem[indice].contaAcesso++; // Conta acesso do endereco recem copiado.
        io->usedPages++;
    }

	io->escritas++;
}

bool encontraEndereco(IO *io, Memoria *mem, unsigned endereco){
    int i;

    for(i = 0; i < io->numPaginas; i++){
		if(mem[i].endereco == endereco){
			mem[i].bitR = 1;
            mem[i].contaAcesso++; // Conta o acesso do endereco encontrado.
			return true;
		}
	}

	return false;
}

void substituiEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t){
	if(strcmp(io->politicaSubs, "lru") == 0){
		LRU(io, mem, endereco);
	}else if(strcmp(io->politicaSubs, "nru") == 0){
		NRU(io, mem, endereco);
	}else if(strcmp(io->politicaSubs, "segunda_chance") == 0){
		Segunda_chance(io, mem, endereco, t);
	}

	io->writebacks++;
}

void escreveEndereco(IO *io, Memoria *mem, unsigned endereco, clock_t t){
	if(io->usedPages < io->numPaginas){
		adicionaEndereco(io, mem, endereco, t);
	}else{
		io->faults++;
		substituiEndereco(io, mem, endereco, t);
	}
}
