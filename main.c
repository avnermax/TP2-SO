#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int operation = 0, reads = 0, writes = 0, pageFault = 0, writeBacks = 0;
/*  Operation: Comandos executados
	Reads: Leituras feitas com sucesso
	Writes: Escritas feitas com sucesso
	PageFault: Falta de página
	WriteBacks: Substituições feitas utilizando-se as políticas
*/
typedef struct page {

	bool referenced;	/* 0 para páginas não referenciadas, 1 para páginas referenciadas */
	bool modify;
	int last_access;
	unsigned int virtual_adress;

}Page;

void Clear_Reference(Page* physic_mem, int frame){
/* Função responsável por "zerar" o bit de referência */
	int i;

	for(i = 0; i < frame; i++)
		physic_mem[i].referenced = 0;

}

void RequeueOldest(Page* physic_mem, int frame, int instant){
/* Função responsável por atualizar o valor do último acesso da página mais antiga na memória */
	int i, menor = 0;

	for(i = 1; i < frame; i++){

		if(physic_mem[i].last_access < physic_mem[menor].last_access)
			menor = i;

	}
	/* De forma abstatra, atualizando o instante do último acesso é como se estivessemos colocando a página no final da fila */
	physic_mem[menor].last_access = instant;

}

int LRU_Policy(Page* physic_mem, int frame){
/* Escolhe a página mais antiga para ser substituida */
	int i, menor = 0;

	for(i = 1; i < frame; i++){

		if(physic_mem[i].last_access < physic_mem[menor].last_access)
			menor = i;

	}

	return menor;
}

int NRU_Policy(Page* physic_mem, int frame){
/* Escolhe a página de menor classe pra ser substituida */
	int i;
	/* Obs: A classe 0 já é procurada na função FillNewPage */
	/* Classe 1*/
	for(i = 0; i < frame; i++){

		if(physic_mem[i].referenced == 0 && physic_mem[i].modify == 1)
			return i; /* Sucesso */

	}

	/* Classe 2*/
	for(i = 0; i < frame; i++){

		if(physic_mem[i].referenced == 1 && physic_mem[i].modify == 0)
			return i; /* Sucesso */

	}

	/* Classe 3 */
	for(i = 0; i < frame; i++){

		if(physic_mem[i].referenced == 1 && physic_mem[i].modify == 1)
			return i; /* Sucesso */
	}
}

int SecondChance_Policy(Page* physic_mem, int frame){
/* A política de segunda chance funciona basicamente como a LRU, porém, de instantes em instantes reenfileiramos a página
   mais velha na memória. Esse processo de reenfileiramento consiste em apenas atualizar o valor de último acesso  */

	int i, menor = 0;

	for(i = 1; i < frame; i++){

		if(physic_mem[i].last_access < physic_mem[menor].last_access)
			menor = i;

	}

	return menor;
}

void FillNewPage(Page* physic_mem, unsigned int adress, int frame, int instant, char* replacement_policy){
/* Função para o preenchimento dos dados em uma página vazia */

	for(int i = 0; i < frame; i++){
		/* Procura a primeira página "vazia" na memória principal e preenchemos com os dados */
		if(physic_mem[i].referenced == 0 && physic_mem[i].modify == 0){

			physic_mem[i].referenced = 1;
			physic_mem[i].modify = 1;
			physic_mem[i].last_access = instant;
			physic_mem[i].virtual_adress = adress;

			writes++;
			operation++;
			return; /* Sucesso */

		}
	}

	int index;

	if(strcmp(replacement_policy, "LRU")){

		index = LRU_Policy(physic_mem, frame);

		physic_mem[index].referenced = 1;
		physic_mem[index].modify = 1;
		physic_mem[index].last_access = instant;
		physic_mem[index].virtual_adress = adress;

	}

	if(strcmp(replacement_policy, "NRU")){

		index = NRU_Policy(physic_mem, frame);

		physic_mem[index].referenced = 1;
		physic_mem[index].modify = 1;
		physic_mem[index].last_access = instant;
		physic_mem[index].virtual_adress = adress;

	}

	if(strcmp(replacement_policy, "SegundaChance")){

		index = SecondChance_Policy(physic_mem, frame);

		physic_mem[index].referenced = 1;
		physic_mem[index].modify = 1;
		physic_mem[index].last_access = instant;
		physic_mem[index].virtual_adress = adress;

	}

	operation++;
	writes++;
	writeBacks++;
	return;

}

void SearchPage(Page* physic_mem, unsigned int adress, int frame, int instant){
/* Função que procura na memória principal se um determinado endereço está presente */

	for(int i = 0; i < frame; i++){
		/* Procura se existe uma página referenciada com aquele endereço */
		if(physic_mem[i].virtual_adress == adress && physic_mem[i].referenced == 1){
			/* Se sim, o instante do último acesso é atualizado */
			physic_mem[i].last_access = instant;

			reads++;
			operation++;
			return;

		}
	}

	/* Se a procura falhar, então ocorreu uma falta de página */
	operation++;
	pageFault++;
	return;

}

int CalculateShift(int page_size){
/* Calcula a quantidade de bits que serão descartados para gerar o endere~ço virtual */
	int tmp = page_size, s = 0;

	while(tmp > 1){

		tmp = tmp >> 1;
		s++;

	}

	return s;
}

unsigned int CalculateVirtualAdress(unsigned int virtual_adress, int s){
/* Faz um Shift de S casas, resultando no endereço virtual */
	unsigned int new;
	new = virtual_adress>>s;

	return new;
}

void PrintTable(Page* physic_mem, int frame){

	int i;

	for(i = 0; i < frame; i++){

		printf(" Referenced: %i\t Modify: %i\t Last Acess: %i\t Virtual Adress: %x\n", physic_mem[i].referenced, physic_mem[i].modify, physic_mem[i].last_access, physic_mem[i].virtual_adress);

	}

}


int main(int argc, char *argv[]){

	char *replacement_policy = argv[1];
	char *file_name = argv[2];
	int page_size = atoi(argv[3]);
	int mem_size = atoi(argv[4]);
	int frame = mem_size / page_size;

	FILE *arq = fopen(file_name, "r");

	unsigned int adress;
	char rw;
	int instant = 0;

	/* É a variável time que regula quando ocorrerá o clean do bit de referência */
	/* Além disso, ela também dita quando a página mais antiga ganhará sua segunda chance (reenfileiramento)*/
	int time = frame/4;

	Page* physic_mem = (Page*) calloc (frame, sizeof(Page));

	if(arq == NULL){

		printf("Erro ao abrir o arquivo!\n");
		exit(1);

	}else{

		while(!feof(arq)){

			fscanf(arq, "%x %c\n", &adress, &rw);
			printf("%x %c\n", adress, rw);

			if(rw == 'W' || rw == 'w'){

				FillNewPage(physic_mem, adress, frame, instant, replacement_policy);
				instant++;

			}else{

				if(rw == 'R' || rw == 'r'){

					SearchPage(physic_mem, adress, frame, instant);

				}
			}

			/* De tempo em tempos, realizamos o reset do bit referencia, caso seja NRU */
			if(strcmp(replacement_policy, "NRU") && instant % time == 0)
				Clear_Reference(physic_mem, frame);

			if(strcmp(replacement_policy, "SegundaChance") && instant % time == 0)
				RequeueOldest(physic_mem, frame, instant);
		}
	}

	PrintTable(physic_mem, frame);

	printf("\nNome do Arquivo: %s\n", file_name);
	printf("Política de substituição: %s\n", replacement_policy);
	printf("Tamanho da Página: %i\n", page_size);
	printf("Tamanho da Memória: %i\n", mem_size);
	printf("Quantidade de Quadros: %i\n", frame);
	printf("Operation: %i\n", operation);
	printf("Reads: %i\n", reads);
	printf("Writes: %i\n", writes);
	printf("PageFaults: %i\n", pageFault);
	printf("WriteBacks: %i\n", writeBacks);

	return 0;
}
