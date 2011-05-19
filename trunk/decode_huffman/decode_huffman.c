/*
 * decode_huffman.c
 *
 *  Created on: May 18, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct no {
	char simbolo;
	int ocorrencias;
	float frequencia_relativa;
	struct no_lista *prox;
	struct no_lista *esq;
	struct no_lista *dir;
} typedef node;

void iniciar_lista (node **lista)
{
	*lista = NULL;
}

void imprimir_lista(node *lista) {

	node *p = lista;

	printf("[INFO]Lista -> ");
	while (p != NULL) {
		printf("%c", p->simbolo);
		printf(" | ");
		printf("%d", p->ocorrencias);
		printf(" | ");
		printf("%f", p->frequencia_relativa);
		printf(" -> ");
		p = p->prox;
	}

	printf("NULL");
}

void inserir_no_final(node **lista, node *n) {

	node *p = *lista;

    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
    }else {
    	while (p->prox != NULL) { // procura o fim da fila
    		p = p->prox;
    	}

    	p->prox = n;
    }
}

node* array_to_lista(node **array, int qtd_nos) {

	node *lista;
	node *p = *array;
	iniciar_lista(&lista);

	if (array == NULL) {
		return NULL;
	}else {
		int i;
		for (i = 0; i < qtd_nos; i++) {
			node *n = calloc(1, sizeof(node));
			n->simbolo = p[i].simbolo;
			n->ocorrencias = p[i].ocorrencias;
			n->frequencia_relativa = p[i].frequencia_relativa;
			n->prox = NULL;
			inserir_no_final(&lista, n);
		}
	}

	free(*array);
	return lista;
}

int main(int argc, char *argv[]) {

//	if(argc != 2) {
//		printf("Parametros inválidos.\n");
//		exit (EXIT_FAILURE);
//	}

	FILE *infile_huftab;
	if ((infile_huftab = fopen("/home/ronie/development/arquivo_comprimido.huftab", "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	// resgatando tabela de ocorrências
	int i, qtd_simbolos = 0;
	fread(&qtd_simbolos, sizeof(int), 1, infile_huftab);
	node *array = calloc(qtd_simbolos, sizeof(node));
	for (i = 0; i < qtd_simbolos; i++) {
		fread(&array[i], sizeof(node), 1, infile_huftab);
	}


	node *lista = array_to_lista(&array, 52);
	imprimir_lista(lista);
	printf("\n\n");

	fclose(infile_huftab);
	exit(EXIT_SUCCESS);
}

