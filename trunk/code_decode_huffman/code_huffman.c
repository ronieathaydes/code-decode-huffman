/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>

struct no {
	char palavra;
	int ocorrencias;
	float frequencia_relativa;
	struct no *prox;
} typedef node;

void iniciar_lista (node **lista)
{
	*lista = NULL;
}

void imprimir_lista(node *lista) {

	node *p = lista;

	printf("lista -> ");
	while (p->prox != NULL) {
		printf("%x", p->palavra);
		printf("/");
		printf("%d", p->ocorrencias);
		printf(" -> ");
		p = p->prox;
	}

	printf(" NULL ");
}

node *localizar_no(node **lista, char palavra) {

	node *p = *lista;

	if (p == NULL) { // verifica se a lista não está vazia
		return NULL;
	}else {
		while (p != NULL && p->palavra != palavra) { // procura o nó que contem a palavra
			p = p-> prox;
		}

		return p;
	}
}

void inserir_no(node **lista, node *n) {

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

void reportar_ocorrencia(node **lista, char palavra) {

	node *p = localizar_no(lista, palavra);

	if (p == NULL) {
		node *n = calloc(1, sizeof(node));
		n->palavra = palavra;
		n->ocorrencias = 1;
		n->prox = NULL;
		inserir_no(lista, n);
	}else {
		p->ocorrencias++;
	}
}

int contar_nos(node *lista) {

	node *p = lista;
	int qtd_nos = 0;

	while (p->prox != NULL) {
		qtd_nos++;
		p = p->prox;
	}

	return qtd_nos;
}

int comparar_nos(node *a, node *b)
{
	return b->ocorrencias - a->ocorrencias;
}

node *buble_sort(node *lista, int qtd_nos) {

	int i, j;
	node temp;

	for (i = 0; i < qtd_nos; i++) {
		for (j = 0; j < qtd_nos; j++) {
			if (lista[i].ocorrencias > lista[j].ocorrencias) {
				temp = lista[i];
				lista[i] = lista[j];
				lista[j] = temp;
			}
		}
	}

	return lista;
}

node *ordenar_desc(node *lista, int qtd_nos) {

	node *lista_ordenada = calloc(qtd_nos, sizeof(node));

	node *p = lista;
	node *q = NULL;
	node *r = lista_ordenada;

	while (p != NULL) { // transferindo os elementos
		q = p;
		r->palavra = p->palavra;
		r->ocorrencias = p->ocorrencias;
		r++;
		p = p->prox;
		free(q);
	}

	//qsort(lista_ordenada, qtd_nos, sizeof(node), (void *)comparar_nos); // ordenando a lista
	lista_ordenada = buble_sort(lista_ordenada, qtd_nos);
	return lista_ordenada;
}


int main(int argc, char *argv[]) {

//	if(argc != 2) {
//		printf("Parametros inválidos.\n");
//		exit (EXIT_FAILURE);
//	}

	argv[1] = "/home/ronie/development/10-Organizando-Arquivos-para-Desempenho-Cont.pdf";  // temporário

	FILE *infile = fopen(argv[1], "rb");

	node *lista;
	iniciar_lista(&lista);

	int qtd_palavras = 0;

	char palavra;
	while (!feof(infile)) {
		fread(&palavra, sizeof(char), 1, infile);
		reportar_ocorrencia(&lista, palavra);
		qtd_palavras++;
	}

	imprimir_lista(lista);
	printf("\n\n");

	int qtd_nos = contar_nos(lista);
	node *lista_ordenada = ordenar_desc(lista, qtd_nos);

	int i = 0;
	for (i = 0; i < qtd_nos; i++) {
		lista_ordenada[i].frequencia_relativa = (float)lista_ordenada[i].ocorrencias / qtd_palavras;
	}

	for (i = 0; i < qtd_nos; i++) {
		printf("%x", lista_ordenada[i].palavra);
		printf("/");
		printf("%d", lista_ordenada[i].ocorrencias);
		printf("/");
		printf("%f", lista_ordenada[i].frequencia_relativa);
		printf(" ");
	}

	fclose(infile);
	exit(EXIT_SUCCESS);
}
