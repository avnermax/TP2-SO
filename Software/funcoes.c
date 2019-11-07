#include "funcoes.h"

/* POLITICAS DE SUBSTITUIÇÃO */
void LRU(IO *io, Memoria *mem, char *endereco){
    
}

void NRU(IO *io, Memoria *mem, char *endereco){

}

void Segunda_chance(IO *io, Memoria *mem, char *endereco){

}

/* FUNÇÕES */
void adicionaEndereco(IO *io, Memoria *mem, char *endereco){
	if(io->usedPages == 0 && io->indice == 0){
        strcpy(mem[io->indice].endereco, endereco);
	}else{
        io->indice++;
        if(io->indice < io->tamMemoria){
            strcpy(mem[io->indice].endereco, endereco);
        }else{
            io->indice = io->tamMemoria; // Para não estourar o valor do indice.
            substituiEndereco(io, mem, endereco);
        }
	}

	if(io->usedPages < io->numPaginas)
		io->usedPages++;

	io->escritas++;
}

bool encontraEndereco(IO *io, Memoria *mem, char *endereco){
    int i;
    for(i = 0; i < io->tamMemoria; i++){
		if(strcmp(mem[i].endereco, endereco) == 0){
			return true;
		}
	}

	return false;
}

void substituiEndereco(IO *io, Memoria *mem, char *endereco){
	if(strcmp(io->politicaSubs, "lru") == 0){
		LRU(io, mem, endereco);
	}else if(strcmp(io->politicaSubs, "nru") == 0){
		NRU(io, mem, endereco);
	}else if(strcmp(io->politicaSubs, "segunda_chance") == 0){
		Segunda_chance(io, mem, endereco);
	}

	io->writebacks++;
}

void escreveEndereco(IO *io, Memoria *mem, char *endereco){
	if(io->usedPages < io->numPaginas){
		adicionaEndereco(io, mem, endereco);
	}else{
		io->faults++;
		substituiEndereco(io, mem, endereco);
	}
}
