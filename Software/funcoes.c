#include "funcoes.h"

/* POLITICAS DE SUBSTITUIÇÃO */
void LRU(IO *io, Memoria *mem, char *endereco){
    int i, menor;

    // Encontra o indice do endereço com menor número de acesso.
    menor = mem[0].contaAcesso;
    for(i = 1; i < io->tamMemoria; i++){
        if(mem[i].contaAcesso < menor){
            menor = mem[i].contaAcesso;
        }
    }

    strcpy(mem[menor].endereco, endereco);
    mem[menor].contaAcesso++; // Conta acesso do endereço recém copiado.
}

void NRU(IO *io, Memoria *mem, char *endereco){

}

void Segunda_chance(IO *io, Memoria *mem, char *endereco){

}

/* FUNÇÕES */
void adicionaEndereco(IO *io, Memoria *mem, char *endereco){
	if(io->usedPages == 0){
        strcpy(mem[io->usedPages].endereco, endereco);
        io->usedPages++;
	}else{
        if(io->usedPages < io->numPaginas){
            strcpy(mem[io->usedPages].endereco, endereco);
            mem[io->usedPages].contaAcesso++; // Conta acesso do endereço recém copiado.
            io->usedPages++;
        }else{
            substituiEndereco(io, mem, endereco);
        }
	}

	io->escritas++;
}

bool encontraEndereco(IO *io, Memoria *mem, char *endereco){
    int i;

    for(i = 0; i < io->tamMemoria; i++){
		if(strcmp(mem[i].endereco, endereco) == 0){
            mem[i].contaAcesso++; // Conta o acesso do endereço encontrado.
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
