/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define TAMANHO_MAX_CODIGO 100

struct no {
	char simbolo;
	int ocorrencias;
	float frequencia_relativa;
	struct no_lista *prox;
	struct no_lista *esq;
	struct no_lista *dir;
} typedef node;

struct {
	char simbolo;
	char codigo[TAMANHO_MAX_CODIGO];
} typedef node_table;

static node_table *tabela = NULL;
static node_table *fim_tabela = NULL;

void iniciar_lista (node **lista)
{
	*lista = NULL;
}

void imprimir_lista(node *lista) {

	node *p = lista;

	printf("[INFO]Lista -> ");
	while (p != NULL) {
//		printf("%x", p->palavra);
//		printf("/");
//		printf("%d", p->ocorrencias);
//		printf("/");
		printf("%f", p->frequencia_relativa);
		printf(" -> ");
		p = p->prox;
	}

	printf("NULL");
}

void imprimir_array(node *array, int qtd_nos)
{
	printf("Array -> ");

	int i;
    for(i = 0; i < qtd_nos; i++){
        printf("%x", array[i].simbolo);
        printf("/");
        printf("%d", array[i].ocorrencias);
//      printf("/");
//      printf("%f", array[i].frequencia_relativa);
        printf("  ");
    }
}

node *localizar_no(node **lista, char simbolo) {

	node *p = *lista;

	if (p == NULL) { // verifica se a lista não está vazia
		return NULL;
	}else {
		while (p != NULL && p->simbolo != simbolo) { // procura o nó que contem a palavra
			p = p-> prox;
		}

		return p;
	}
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

node *remover_no_final(node **lista) {

	node *p = *lista;

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

void reportar_ocorrencia(node **lista, char simbolo) {

	node *p = localizar_no(lista, simbolo);

	if (p == NULL) {
		node *n = calloc(1, sizeof(node));
		n->simbolo = simbolo;
		n->ocorrencias = 1;
		n->frequencia_relativa = 0;
		n->prox = NULL;
		n->esq = NULL;
		n->dir = NULL;
		inserir_no_final(lista, n);
	}else {
		p->ocorrencias++;
	}
}

int contar_simbolos(node *lista) {

	node *p = lista;
	int qtd_simbolos = 0;

	while (p->prox != NULL) {
		qtd_simbolos++;
		p = p->prox;
	}

	return qtd_simbolos;
}

//int comparar_nos(node *a, node *b)
//{
//	return b->ocorrencias - a->ocorrencias;
//}

void buble_sort(node *array, int qtd_nos) {

	int i, j;
	node temp;

	for (i = 0; i < qtd_nos; i++) {
		for (j = i; j < qtd_nos; j++) {
			if (array[i].ocorrencias > array[j].ocorrencias) {
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

node *lista_to_array(node *lista, int qtd_nos) {

	node *array = calloc(qtd_nos, sizeof(node));

	node *p = lista;
	node *q = NULL;
	node *r = array;

	while (p->prox != NULL) { // transferindo os elementos
		q = p;
		r->simbolo = p->simbolo;
		r->ocorrencias = p->ocorrencias;
		r->frequencia_relativa = p->frequencia_relativa;
		r++;
		p = p->prox;
		free(q);
	}

	iniciar_lista(lista);
	return array;
}

node *array_to_lista(node **array, int qtd_nos) {

	node *lista, *p = *array;
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

node *ordenar_lista(node **lista, int qtd_nos) {

	node *array = lista_to_array(*lista, qtd_nos);

//	qsort(lista_ordenada, qtd_nos, sizeof(node), (void *)comparar_nos); // ordenando a lista
	buble_sort(array, qtd_nos); // ordenando a lista

	lista = array_to_lista(&array, qtd_nos);
	return lista;
}

void setar_frequencias(node **lista, int qtd_palavras) {

	node *p = *lista;
	while (p != NULL) {
		p->frequencia_relativa = (float)p->ocorrencias / qtd_palavras;
		p = p->prox;
	}
}

node *gerar_arvore(node **lista) {

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
		} while ((p->prox != NULL)); // na última passada a lista contém somente a raiz da árvore, o processo está completo. VERIFICAR!!!

		return n;
	}
}

void inserir_elemento(char simbolo, char codigo[]) {
	fim_tabela->simbolo = simbolo;
	stpcpy(fim_tabela->codigo, codigo);
	fim_tabela++;
}

void gerar_codigos(node *lista, char codigo[]) {

	char s0[2] = "0";
	char s1[2] = "1";

	if (lista != NULL) {
		if ((lista->esq == NULL) && (lista->dir == NULL)) {
			inserir_elemento(lista->simbolo, codigo);
#ifdef DEBUG
			printf("[DEBUG]Foi inserido o par [%x | %s]", lista->simbolo, codigo);
			printf("\n");
#endif
		}

		int tamanho = 0;
		gerar_codigos(lista->esq, strcat(codigo, s0));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
		gerar_codigos(lista->dir, strcat(codigo, s1));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
	}
}

void imprimir_tabela_codigos(int qtd_simbolos) {

	int i;

	printf("[INFO]Tabela de frequência -> ");
	for(i = 0; i < qtd_simbolos; i++){
		printf("[%x", tabela[i].simbolo);
		printf("|");
		printf("%s]", tabela[i].codigo);
		printf(" ");
	}
}

char *get_codigo(char simbolo) {

	node_table *p = tabela;
	char codigo[TAMANHO_MAX_CODIGO];

	while (p != fim_tabela) {
		if (p->simbolo == simbolo) {
			strcpy(codigo, p->codigo);
			return codigo;
		}
		else {
			p++;
		}
	}
}

int main(int argc, char *argv[]) {

//	if(argc != 2) {
//		printf("Parametros inválidos.\n");
//		exit (EXIT_FAILURE);
//	}

//	argv[1] = "/home/ronie/development/10-Organizando-Arquivos-para-Desempenho-Cont.pdf";
//	argv[1] = "/home/ronie/development/musica.mp3";                                             // temporário
//	argv[1] = "/home/ronie/development/senha";
	argv[1] = "/home/ronie/development/teste";

	FILE *infile;
	if ((infile = fopen(argv[1], "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	node *lista;
	iniciar_lista(&lista);

	int qtd_palavras = 0;

	char palavra;
	while (!feof(infile)) {
		fread(&palavra, sizeof(char), 1, infile);
		reportar_ocorrencia(&lista, palavra);
		qtd_palavras++;
	}

	int qtd_simbolos = contar_simbolos(lista);
	lista = ordenar_lista(&lista, qtd_simbolos);
	setar_frequencias(&lista, qtd_simbolos);

#ifdef DEBUG
	imprimir_lista(lista);
	printf("\n\n");
#endif

	tabela = calloc(qtd_simbolos, sizeof(node_table));
	fim_tabela = tabela;

	char vazio[1] = "";
	node *arvore = gerar_arvore(&lista);
	gerar_codigos(arvore, vazio);

#ifdef DEBUG
	printf("\n");
	imprimir_tabela_codigos(qtd_simbolos);
	printf("\n\n");
#endif

	FILE *outfile_huff;
	if ((outfile_huff = fopen("/home/ronie/development/arquivo_comprimido.huff", "wb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	rewind(infile); // resetando o arquivo de entrada

	char *codigo[TAMANHO_MAX_CODIGO];
	while (!feof(infile)) {
		fread(&palavra, sizeof(char), 1, infile);

		*codigo = get_codigo(palavra);

		fwrite(codigo, sizeof(char), TAMANHO_MAX_CODIGO, outfile_huff);
	}

#ifdef DEBUG
	printf("FUNFOU!!!");
#endif

	fclose(infile);
	fclose(outfile_huff);
	exit(EXIT_SUCCESS);
}
