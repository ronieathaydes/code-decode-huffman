/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char palavra;
	int ocorrencias;
	struct node *prox;
};

void inserir_fila(node *fila, char palavra) {

	node *f;
	node *p = *fila;

	p = calloc(1, sizeof(node));
	p->palavra = palavra;
	p->ocorrencias = 0;
	p->prox = NULL;

    // verifica se a lista não está vazia
    if(p == NULL) {
    	*fila = *f;
    }else {
    	while (f != NULL) {

    	}
    }
}


int verificar_existencia() {
	return 0;
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Parametros inválidos.\n");
		return(0);
	}

	FILE *infile = fopen(argv[1], "rb");

	char byte;
	while (feof(infile)) {
		fread(&byte, sizeof(char), 1, infile);
	}

}
