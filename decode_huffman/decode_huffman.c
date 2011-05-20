/*
 * decode_huffman.c
 *
 *  Created on: May 18, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT (char)0x01
#define DEBUG

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

void setar_frequencias(node **lista, int total_simbolos) {

	node *p = *lista;
	while (p != NULL) {
		p->frequencia_relativa = (float)p->ocorrencias / total_simbolos;
		p = p->prox;
	}
}

void inserir_no_ordenado(node **lista, node *n) {

	node *p, *q;
#ifdef DEBUG
	int i = 0;
#endif

	p = *lista;
    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
#ifdef DEBUG
    	printf("[DEBUG]O nó foi inserido no início da lista (a lista estava vazia).");
    	printf("\n");
#endif
    }else {
    	while ((p != NULL) && (p->frequencia_relativa < n->frequencia_relativa)) { // procura a posição de inserção do novo nó
    		q = p;
    		p = p->prox;
#ifdef DEBUG
    		i++;
#endif
    	}

#ifdef DEBUG
    	printf("[DEBUG]Foram deslocados %d nós", i);
    	printf("\n");
#endif

    	if (p == *lista) { // o novo nó será o primeiro da lista
			*lista = n;
			n->prox = p;
#ifdef DEBUG
    		printf("[DEBUG]O nó foi inserido no início da lista");
    		printf("\n");
#endif
    	}else if (p == NULL) { // o novo no será o último da lista
    		q->prox = n;
    		n->prox = NULL;
#ifdef DEBUG
    		printf("[DEBUG]O nó foi inserido no fim da lista");
    		printf("\n");
#endif
    	}else { // o novo nó será inserido no meio da lista
    		q->prox = n;
    		n->prox = p;
#ifdef DEBUG
    		printf("[DEBUG]O nó foi inserido no meio da lista");
    		printf("\n");
#endif
    	}
    }
}

node* gerar_arvore(node **lista) {

	node *n, *q, *p = *lista;
#ifdef DEBUG
	int i = 0;
#endif

	if (p == NULL) {
		return NULL;
	}else if (p->prox == NULL) {
		return p;
		p->prox = NULL;
		p->esq = NULL;
		p->dir = NULL;
	}else {
		do {
			q = p->prox;
			*lista = q->prox;

			n = calloc(1, sizeof(node));
			n->frequencia_relativa = p->frequencia_relativa + q->frequencia_relativa;
			n->esq = p;
			n->dir = q;

			p->prox = NULL;
			q->prox = NULL;

#ifdef DEBUG
			printf("[DEBUG]%dº loop", ++i);
			printf("\n");
#endif
			inserir_no_ordenado(lista, n);
#ifdef DEBUG
			imprimir_lista(*lista);
			printf("\n\n");
#endif
			p->prox = NULL;
			q->prox = NULL;

			p = *lista;
		} while ((p->prox != NULL));

		return n;
	}
}

void decodificar_arquivo(FILE *infile_huftab, FILE *outfile, node *arvore) {

	char simbolo;

	int count_buffer = 0;
	unsigned char buffer;

	node *p = arvore;
	fread(&buffer, sizeof(char), 1, infile_huftab);

	while (!feof(infile_huftab)) {

		while (((p->esq != NULL) && (p->dir != NULL)) || (count_buffer < 8)) {
			if (((BIT << count_buffer) & buffer) == 0) {
				p = p->esq;
			}else {
				p = p->dir;
			}

			count_buffer++;
		}

		if ((p->esq != NULL) && (p->dir != NULL)) {
			simbolo = p->simbolo;
			fwrite(&simbolo, sizeof(char), 1, outfile);
			p = arvore;
		}else if (count_buffer == 8) {
			fread(&buffer, sizeof(char), 1, infile_huftab);
			count_buffer = 0;
		}
	}
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

	FILE *outfile;
	if ((outfile = fopen("/home/ronie/development/arquivo_comprimido.huftab", "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	// resgatando total de símbolos e tabela de ocorrências
	int i, qtd_simbolos, total_simbolos = 0;
	fread(&total_simbolos, sizeof(int), 1, infile_huftab);
	fread(&qtd_simbolos, sizeof(int), 1, infile_huftab);
	node *array = calloc(qtd_simbolos, sizeof(node));
	for (i = 0; i < qtd_simbolos; i++) {
		fread(&array[i], sizeof(node), 1, infile_huftab);
	}

	node *lista = array_to_lista(&array, qtd_simbolos);
	setar_frequencias(&lista, total_simbolos);

	node *arvore = gerar_arvore(&lista);
	decodificar_arquivo(infile_huftab, outfile, arvore);

	fclose(infile_huftab);
	fclose(outfile);
	exit(EXIT_SUCCESS);
}

