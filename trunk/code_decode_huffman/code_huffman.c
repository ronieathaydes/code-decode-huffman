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
	struct no *prox;
} typedef node;

void iniciar_fila (node **fila)
{
	*fila = NULL;
}

node *localizar_no(node **fila, char palavra) {

	node *p = *fila;

	if (p == NULL) { // verifica se a lista não está vazia
		return NULL;
	}else {
		while (p != NULL && p->palavra != palavra) { // procura o nó que contem a palavra
			p = p-> prox;
		}

		return p;
	}
}

void inserir_no(node **fila, node *n) {

	node *p = *fila;

    if (p == NULL) { // verifica se a lista não está vazia
    	*fila = n;
    }else {
    	while (p->prox != NULL) { // procura o fim da fila
    		p = p->prox;
    	}

    	p->prox = n;
    }
}

void reportar_ocorrencia(node **fila, char palavra) {

	node *p = localizar_no(fila, palavra);

	if (p == NULL) {
		node *n = calloc(1, sizeof(node));
		n->palavra = palavra;
		n->ocorrencias = 1;
		n->prox = NULL;
		inserir_no(fila, n);
	}else {
		p->ocorrencias++;
	}
}

//node *ordenar_fila(node *fila) {
//
//	node *fila_ordenada;
//	iniciar_fila(&fila_ordenada);
//
//	node *p = fila;
//	node *anterior_p = NULL;
//
//	node *maior = fila;
//	node *anterior_maior = NULL;
//
//	while (fila != NULL) {
//		p = fila;
//		while (p != NULL) {
//			if (p->prox == NULL) {
//				maior = p;
//				fila->prox = NULL;
//			}else {
//				if (p->ocorrencias > maior->ocorrencias) {
//					anterior_maior = anterior_p;
//					maior = p;
//				}
//
//				anterior_p = p;
//				p = p->prox;
//
//			}
//		}
//
//		anterior_maior->prox = maior->prox;
//		maior->prox = NULL;
//		inserir_no(&fila_ordenada, maior); // inserindo nó com mais ocorrências na lista ordenada
//	}
//
//	return fila_ordenada;
//}

//int ordenar_fila(node *fila){
//
//	int qtd_nos = 0;
//	node *p = fila;
//
//	while (p->prox != NULL) {
//		qtd_nos++;
//		p = p->prox;
//	}
//
//	node vetor[qtd_nos];
//}

void imprimir_fila(node *fila) {

	node *p = fila;

	printf("fila -> ");
	while (p->prox != NULL) {
		printf("%c", p->palavra);
		printf("/");
		printf("%d", p->ocorrencias);
		printf(" -> ");
		p = p->prox;
	}

	printf(" NULL ");
}

int main(int argc, char *argv[]) {

//	if(argc != 2) {
//		printf("Parametros inválidos.\n");
//		exit (0);
//	}

	argv[1] = "/home/ronie/development/senha";  // temporário

	FILE *infile = fopen(argv[1], "rb");

	node *fila;
	iniciar_fila(&fila);

	char palavra;
	while (!feof(infile)) {
		fread(&palavra, sizeof(char), 1, infile);
		reportar_ocorrencia(&fila, palavra);
	}

	imprimir_fila(fila);
	printf("\n\n");
	printf("%d", ordenar_fila(fila));
//	fila = ordenar_fila(fila);
//	imprimir_fila(fila);

	fclose(infile);
	exit(0);
}
