#include "funcoes.h"

/* POLITICAS DE SUBSTITUIÃ‡ÃƒO */
void LRU(IO *io, Memoria *mem, char *endereco){
    int i, menor;

    // Encontra o indice do endereÃ§o com menor nÃºmero de acesso.
    menor = mem[0].contaAcesso;
    for(i = 1; i < io->tamMemoria; i++){
        if(mem[i].contaAcesso < menor){
            menor = mem[i].contaAcesso;
        }
    }

    strcpy(mem[menor].endereco, endereco);
    mem[menor].contaAcesso++; // Conta acesso do endereÃ§o recÃ©m copiado.
}

void NRU(IO *io, Memoria *mem, char *endereco){
    
}

void Segunda_chance(IO *io, Memoria *mem, char *endereco){
    int i, c = 0;
    float menor;

    // Encontra o indice do endereÃ§o com o acesso mais antigo.
    menor = mem[0].clckacesso;
    for(i = 1; i < io->tamMemoria; i++){
        if(mem[i].clckacesso < menor){
			if(mem[i].bitR == 0){
				menor = mem[i].clckacesso;
	    		c = i;
			}else{
				mem[c].bitR = 0;
			}			
        }
    }
    strcpy(mem[c].endereco, endereco);
    mem[c].clckacesso = (clock() - tempo) / (float)CLOCKS_PER_SEC;
	mem[c].bitR = 0;	
    mem[menor].contaAcesso++; // Conta acesso do endereÃ§o recÃ©m copiado.
    
}

/* FUNÇÕES */
void adicionaEndereco(IO *io, Memoria *mem, char *endereco){
	if(io->usedPages == 0){
        strcpy(mem[io->usedPages].endereco, endereco);
		mem[io->usedPages].clckacesso = (clock() - tempo) / (float)CLOCKS_PER_SEC;
		mem[io->usedPages].bitR = 1 ;
        io->usedPages++;
	}else{
        if(io->usedPages < io->numPaginas){
            strcpy(mem[io->usedPages].endereco, endereco);
	    	mem[io->usedPages].clckacesso = (clock() - tempo) / (float)CLOCKS_PER_SEC;
	    	mem[io->usedPages].bitR = 1 ;
            mem[io->usedPages].contaAcesso++; // Conta acesso do endereÃ§o recÃ©m copiado.
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
			mem[i].bitR =1;
            mem[i].contaAcesso++; // Conta o acesso do endereÃ§o encontrado.
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
