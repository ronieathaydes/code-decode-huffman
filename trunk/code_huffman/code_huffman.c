/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>

struct no_lista {
	char palavra;
	int ocorrencias;
	float frequencia_relativa;
	struct no_lista *prox;
} typedef node_lista;

struct no_arvore {
	struct no_lista *esq;
	struct no_lista *dir;
} typedef node_arvore;

void iniciar_lista (node_lista **lista)
{
	*lista = NULL;
}

void imprimir_lista(node_lista *lista) {

	node_lista *p = lista;

	printf("Lista -> ");
	while (p->prox != NULL) {
		printf("%x", p->palavra);
		printf("/");
		printf("%d", p->ocorrencias);
		printf("/");
		printf("%f", p->frequencia_relativa);
		printf(" -> ");
		p = p->prox;
	}

	printf(" NULL ");
}

void imprimir_array(node_lista *array, int qtd_nos)
{
	printf("Array -> ");

	int i;
    for(i = 0; i < qtd_nos; i++){
        printf("%x", array[i].palavra);
        printf("/");
        printf("%d", array[i].ocorrencias);
//      printf("/");
//      printf("%f", array[i].frequencia_relativa);
        printf("  ");
    }
}

node_lista *localizar_no(node_lista **lista, char palavra) {

	node_lista *p = *lista;

	if (p == NULL) { // verifica se a lista não está vazia
		return NULL;
	}else {
		while (p != NULL && p->palavra != palavra) { // procura o nó que contem a palavra
			p = p-> prox;
		}

		return p;
	}
}

void inserir_no_final(node_lista **lista, node_lista *n) {

	node_lista *p = *lista;

    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
    }else {
    	while (p->prox != NULL) { // procura o fim da fila
    		p = p->prox;
    	}

    	p->prox = n;
    }
}

void inserir_no_ordenado(node_lista **lista, node_lista *n) {

	node_lista *p, *q;

	p = *lista;
    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
    }else {
    	while ((p->prox != NULL) && (p->ocorrencias < n->ocorrencias)) { // procura a posição de inserção do novo nó
    		q = p;
    		p = p->prox;
    	}

    	if (p == *lista) { // p é o primeiro da lista
    		if (p->ocorrencias < n->ocorrencias) {
    			p->prox = n;
    			n->prox = NULL;
    		}else {
    			*lista = n;
    			n->prox = p;
    			p->prox = NULL;
    		}
    	}else if (p->prox == NULL) { // p é o ultimo da lista
    		p->prox = n;
    		n->prox = NULL;
    	}else {
    		q->prox = n;
    		n->prox = p;
    	}
    }
}

node_lista *remover_no_final(node_lista **lista) {

	node_lista *p = *lista;

    if (p == NULL) { // verifica se a lista não está vazia
    	return NULL;
    }else {
    	if (p->prox == NULL) {
    		*lista = NULL;
    	}else {
    		*lista = p->prox;
    	}

    	return p;
    }
}

void reportar_ocorrencia(node_lista **lista, char palavra) {

	node_lista *p = localizar_no(lista, palavra);

	if (p == NULL) {
		node_lista *n = calloc(1, sizeof(node_lista));
		n->palavra = palavra;
		n->ocorrencias = 1;
		n->frequencia_relativa = 0;
		n->prox = NULL;
		inserir_no_final(lista, n);
	}else {
		p->ocorrencias++;
	}
}

int contar_nos(node_lista *lista) {

	node_lista *p = lista;
	int qtd_nos = 0;

	while (p->prox != NULL) {
		qtd_nos++;
		p = p->prox;
	}

	return qtd_nos;
}

//int comparar_nos(node *a, node *b)
//{
//	return b->ocorrencias - a->ocorrencias;
//}

void buble_sort(node_lista *array, int qtd_nos) {

	int i, j;
	node_lista temp;

	for (i = 0; i < qtd_nos; i++) {
		for (j = i; j < qtd_nos; j++) {
			if (array[i].ocorrencias < array[j].ocorrencias) {
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

node_lista *lista_to_array(node_lista *lista, int qtd_nos) {

	node_lista *array = calloc(qtd_nos, sizeof(node_lista));

	node_lista *p = lista;
	node_lista *q = NULL;
	node_lista *r = array;

	while (p->prox != NULL) { // transferindo os elementos
		q = p;
		r->palavra = p->palavra;
		r->ocorrencias = p->ocorrencias;
		r->frequencia_relativa = p->frequencia_relativa;
		r++;
		p = p->prox;
		free(q);
	}

	iniciar_lista(lista);
	return array;
}

node_lista *array_to_lista(node_lista **array, int qtd_nos) {

	node_lista *lista, *p = *array;
	iniciar_lista(&lista);

	if (array == NULL) {
		return NULL;
	}else {
		int i;
		for (i = 0; i < qtd_nos; i++) {
			node_lista *n = calloc(1, sizeof(node_lista));
			n->palavra = p[i].palavra;
			n->ocorrencias = p[i].ocorrencias;
			n->frequencia_relativa = p[i].frequencia_relativa;
			n->prox = NULL;
			inserir_no_final(&lista, n);
		}
	}

	free(*array);
	return lista;
}

node_lista *ordenar_lista(node_lista **lista, int qtd_nos) {

	node_lista *array = lista_to_array(*lista, qtd_nos);

//	qsort(lista_ordenada, qtd_nos, sizeof(node), (void *)comparar_nos); // ordenando a lista
	buble_sort(array, qtd_nos); // ordenando a lista

	lista = array_to_lista(&array, qtd_nos);
	return lista;
}

void setar_frequencias(node_lista **lista, int qtd_palavras) {

	node_lista *p = *lista;
	while (p->prox != NULL) {
		p->frequencia_relativa = (float)p->ocorrencias / qtd_palavras;
		p = p->prox;
	}
}

int main(int argc, char *argv[]) {

//	if(argc != 2) {
//		printf("Parametros inválidos.\n");
//		exit (EXIT_FAILURE);
//	}

	argv[1] = "/home/ronie/development/10-Organizando-Arquivos-para-Desempenho-Cont.pdf";
//	argv[1] = "/home/ronie/development/musica.mp3";                                             // temporário
//	argv[1] = "/home/ronie/development/senha";

	FILE *infile = fopen(argv[1], "r");

	node_lista *lista;
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
	lista = ordenar_lista(&lista, qtd_nos);
	setar_frequencias(&lista, qtd_palavras);

	imprimir_lista(lista);
	printf("\n\n");



//	para i $ 1 ate´ n # 1 fac¸ a
//		alocar novo registro z; ! vertice de ´ T
//		z.esq $ x $ EXTRAI MIN(Q);
//		z.dir $ y $ EXTRAI MIN(Q);
//		z.f $ x.f + y.f ;
//		INSERE(Q, z);
//		retorne EXTRAI MIN(Q)



	fclose(infile);
	exit(EXIT_SUCCESS);
}
