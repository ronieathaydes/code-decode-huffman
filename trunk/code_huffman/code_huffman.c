/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#define TAMANHO_MAX_CODIGO 300
#define FORMATO_IMPRESSAO "%c"

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
		printf(FORMATO_IMPRESSAO, p->simbolo);
		printf(" | ");
		printf("%d", p->ocorrencias);
		printf(" | ");
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

node* localizar_no(node **lista, char simbolo) {

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

node* remover_no_final(node **lista) {

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

	while (p != NULL) {
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

node* lista_to_array(node *lista, int qtd_nos) {

	node *array = calloc(qtd_nos, sizeof(node));

	node *p = lista;
	node *q = NULL;
	node *r = array;

	while (p != NULL) { // transferindo os elementos
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

node* array_to_lista(node **array, int qtd_nos) {

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

node* ordenar_lista(node **lista, int qtd_nos) {

	node *array = lista_to_array(*lista, qtd_nos);

//	qsort(lista_ordenada, qtd_nos, sizeof(node), (void *)comparar_nos); // ordenando a lista
	buble_sort(array, qtd_nos); // ordenando a lista

	lista = array_to_lista(&array, qtd_nos);
	return lista;
}

void setar_frequencias(node **lista, int total_simbolos) {

	node *p = *lista;
	while (p != NULL) {
		p->frequencia_relativa = (float)p->ocorrencias / total_simbolos;
		p = p->prox;
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

void inserir_elemento(char simbolo, char codigo[]) {
	fim_tabela->simbolo = simbolo;
	stpcpy(fim_tabela->codigo, codigo);
	fim_tabela++;
}

void gerar_codigos(node *lista, char *codigo) {

	char *c0 = "0";
	char *c1 = "1";

	if (lista != NULL) {
		if ((lista->esq == NULL) && (lista->dir == NULL)) {
			inserir_elemento(lista->simbolo, codigo);
#ifdef DEBUG
			char msg[100];
			msg[0]= '\0';
			strcat(msg, "[DEBUG]Foi inserido o par [");
			strcat(msg, FORMATO_IMPRESSAO);
			strcat(msg, " | %s]");
			printf(msg, lista->simbolo, codigo);
			printf("\n");
#endif
		}

		int tamanho = 0;
		gerar_codigos(lista->esq, strcat(codigo, c0));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
		gerar_codigos(lista->dir, strcat(codigo, c1));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
	}
}

void imprimir_tabela_codigos(int qtd_simbolos) {

	int i;

	char formato[3];
	formato[0] = '\0';
	strcat(formato, "[");
	strcat(formato, FORMATO_IMPRESSAO);

	printf("[INFO]Tabela de codigos -> ");
	for(i = 0; i < qtd_simbolos; i++){
		printf(formato, tabela[i].simbolo);
		printf("|");
		printf("%s]", tabela[i].codigo);
		printf(" ");
	}
}

char* get_codigo(char simbolo) {

	node_table *p = tabela;

	while (p != fim_tabela) {
		if (p->simbolo == simbolo) {
			return p->codigo;
		}
		else {
			p++;
		}
	}

	return NULL;
}

void codificar_arquivo(FILE *infile, FILE *outfile_huff, FILE *outfile_huftab) {

	char *codigo;
	int tamanho_codigo = 0;
	char c0 = '0';
	char c1 = '1';

	int count_buffer = 0;
	unsigned char buffer = 0;

	char simbolo;
	while (!feof(infile)) {
		fread(&simbolo, sizeof(char), 1, infile);
		codigo = get_codigo(simbolo);

#ifdef DEBUG
		printf("[INFO]Palavra: %c | Código: %s", simbolo, codigo);
		printf("\n");
#endif

		int i;
		tamanho_codigo = strlen(codigo);
		for (i = 0; i < tamanho_codigo; i++) {
			if (codigo[i] == c0) {
				buffer = (buffer << 1) | 0;
			}else if (codigo[i] == c1) {
				buffer = (buffer << 1) | 1;
			}

			count_buffer++;
#ifdef DEBUG
		printf("[INFO]CountBuffer = %d", count_buffer);
			printf("\n");
#endif
			if (count_buffer == 8) {
				fwrite(&buffer, sizeof(char), 1, outfile_huff);
				fwrite(&buffer, sizeof(char), 1, outfile_huftab);
				buffer = 0;
				count_buffer = 0;
#ifdef DEBUG
				printf("[INFO]Esvaziando buffer...");
				printf("\n");
#endif
			}
		}
	}

	if (count_buffer > 0) {
		fwrite(&buffer, sizeof(char), 1, outfile_huff);
		buffer = buffer << 8;
		count_buffer = 0;
#ifdef DEBUG
		printf("[INFO]Esvaziando buffer...");
		printf("\n");
#endif
	}
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Parametros inválidos.\n");
		exit (EXIT_FAILURE);
	}

//	argv[1] = "/home/ronie/development/lero-lero.txt";
//	argv[1] = "/home/ronie/development/teste.txt";

	FILE *infile;
	if ((infile = fopen(argv[1], "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

//	/home/ronie/development/

	FILE *outfile_huff;
	if ((outfile_huff = fopen("arquivo_comprimido.huff", "wb")) == NULL) {
		printf("O arquivo de saída não pode ser criado.\n");
		exit(EXIT_FAILURE);
	}

	FILE *outfile_huftab;
	if ((outfile_huftab = fopen("arquivo_comprimido.huftab", "wb")) == NULL) {
		printf("O arquivo de saída não pode ser criado.\n");
		exit(EXIT_FAILURE);
	}

//	{
//		TRATAMENTO DO NOME DO ARQUIVO
//		int i = 0;
//		char *str = argv[1];
//		char nome_arquivo[50];nome_arquivo[0] = '\0';
//		int count = strlen(str);
//		int inicio = strlen(str);
//		for (i = count; i >= 1; i--) {
//			if (str[i] == '/') {
//				break;
//			}else {
//				inicio--;
//			}
//		}
//
//		char *c = NULL;
//		for (i = inicio; i < count; i++) {
//			c = str[i];
//			strcat(nome_arquivo, *c);
//			i++;
//		}
//
//		printf("%s", nome_arquivo);
//		printf("\n\n");
//	}

	node *lista;
	iniciar_lista(&lista);

	char simbolo;
	int total_simbolos = 0;
	while (!feof(infile)) {
		fread(&simbolo, sizeof(char), 1, infile);
		reportar_ocorrencia(&lista, simbolo);
		total_simbolos++;
	}

	int qtd_simbolos = contar_simbolos(lista);
	lista = ordenar_lista(&lista, qtd_simbolos);

	// persistindo total de símbolos e tabela de ocorrências
	fwrite(&total_simbolos, sizeof(int), 1, outfile_huftab);
	fwrite(&qtd_simbolos, sizeof(int), 1, outfile_huftab);
	node *array = lista_to_array(lista, qtd_simbolos);
	int i;
	for (i = 0; i < qtd_simbolos; i++) {
		fwrite(&array[i], sizeof(node), 1, outfile_huftab);
	}

	lista = array_to_lista(&array, qtd_simbolos);
	setar_frequencias(&lista, total_simbolos);

	tabela = calloc(qtd_simbolos, sizeof(node_table));
	fim_tabela = tabela;

	node *arvore = gerar_arvore(&lista);

	char vazio[TAMANHO_MAX_CODIGO];vazio[0] = '\0';
	gerar_codigos(arvore, vazio);

	// resetando o arquivo de entrada
	rewind(infile);

	codificar_arquivo(infile, outfile_huff, outfile_huftab);

#ifdef DEBUG
	printf("\n");
	printf("FUNFOU!!!");
#endif

	fclose(infile);
	fclose(outfile_huff);
	fclose(outfile_huftab);
	exit(EXIT_SUCCESS);
}
