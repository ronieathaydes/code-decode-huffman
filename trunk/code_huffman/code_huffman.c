/*
 * code_huffman.c
 *
 *  Created on: May 12, 2011
 *      Author: ronie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_MAX_CODIGO 150
#define TAMANHO_MAX_PATH 100
#define TAMANHO_MAX_NOME_ARQUIVO 50

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

	p = *lista;
    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
    }else {
    	while ((p != NULL) && (p->frequencia_relativa < n->frequencia_relativa)) { // procura a posição de inserção do novo nó
    		q = p;
    		p = p->prox;
    	}

    	if (p == *lista) {     // o novo nó será o primeiro da lista
			*lista = n;
			n->prox = p;
    	}else if (p == NULL) { // o novo no será o último da lista
    		q->prox = n;
    		n->prox = NULL;
    	}else {                // o novo nó será inserido no meio da lista
    		q->prox = n;
    		n->prox = p;
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

			inserir_no_ordenado(lista, n);
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
		}

		int tamanho = 0;
		gerar_codigos(lista->esq, strcat(codigo, c0));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
		gerar_codigos(lista->dir, strcat(codigo, c1));
		tamanho = strlen(codigo);codigo[tamanho-1] = '\0';
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

		int i;
		tamanho_codigo = strlen(codigo);
		for (i = 0; i < tamanho_codigo; i++) {
			if (codigo[i] == c0) {
				buffer = (buffer << 1) | 0;
			}else if (codigo[i] == c1) {
				buffer = (buffer << 1) | 1;
			}

			count_buffer++;
			if (count_buffer == 8) {
				fwrite(&buffer, sizeof(char), 1, outfile_huff);
				fwrite(&buffer, sizeof(char), 1, outfile_huftab);
				buffer = 0;
				count_buffer = 0;
			}
		}
	}

	if (count_buffer > 0) {
		fwrite(&buffer, sizeof(char), 1, outfile_huff);
		buffer = buffer << 8;
		count_buffer = 0;
	}
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Parametros inválidos.\n");
		exit (EXIT_FAILURE);
	}

	char *inpath = argv[1];

	FILE *infile;
	if ((infile = fopen(argv[1], "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	// encontrando a última "\" ou "/" do path
	char *final_path = NULL;
	final_path = strrchr(inpath, '/');
	if (final_path == NULL) {
		*final_path = strrchr(inpath, '\\');
	}

	// definindo o path do arquivo de saída para o mesmo do arquivo de entrada
	char outpath[TAMANHO_MAX_PATH] = "";
	strncpy(outpath, inpath, (strlen(inpath) - strlen(final_path) + 1));

	// juntando o path ao nome do arquivo .huff para abrí-lo
	char outfile_huff_path[TAMANHO_MAX_PATH + TAMANHO_MAX_NOME_ARQUIVO] = "";
	strcat(outfile_huff_path, outpath);
	strcat(outfile_huff_path, "arquivo_comprimido.huff");
	FILE *outfile_huff;
	if ((outfile_huff = fopen(outfile_huff_path, "wb")) == NULL) {
		printf("O arquivo de saída não pode ser criado.\n");
		exit(EXIT_FAILURE);
	}

	// juntando o path ao nome do arquivo .huftab para abrí-lo
	char outfile_huftab_path[TAMANHO_MAX_PATH + TAMANHO_MAX_NOME_ARQUIVO] = "";
	strcat(outfile_huftab_path, outpath);
	strcat(outfile_huftab_path, "arquivo_comprimido.huftab");
	FILE *outfile_huftab;
	if ((outfile_huftab = fopen(outfile_huftab_path, "wb")) == NULL) {
		printf("O arquivo de saída não pode ser criado.\n");
		exit(EXIT_FAILURE);
	}

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

	// resgatando nome do arquivo
	char nome_arquivo[TAMANHO_MAX_NOME_ARQUIVO] = "";
	strcpy(nome_arquivo, ++final_path);

	// persistindo nome do arquivo, total de símbolos e tabela de ocorrências
	int i;
	for (i = 0; i < TAMANHO_MAX_NOME_ARQUIVO; i++) {
		fwrite(&nome_arquivo[i], sizeof(char), 1, outfile_huftab);
	}
	fwrite(&total_simbolos, sizeof(int), 1, outfile_huftab);
	fwrite(&qtd_simbolos, sizeof(int), 1, outfile_huftab);
	node *array = lista_to_array(lista, qtd_simbolos);
	for (i = 0; i < qtd_simbolos; i++) {
		fwrite(&array[i], sizeof(node), 1, outfile_huftab);
	}

	lista = array_to_lista(&array, qtd_simbolos);
	setar_frequencias(&lista, total_simbolos);

	tabela = calloc(qtd_simbolos, sizeof(node_table));
	fim_tabela = tabela;

	node *arvore = gerar_arvore(&lista);

	char vazio[TAMANHO_MAX_CODIGO] = "";
	gerar_codigos(arvore, vazio);

	// resetando o arquivo de entrada
	rewind(infile);

	codificar_arquivo(infile, outfile_huff, outfile_huftab);

	long size_infile = ftell(infile) + 1;
	long size_outfile_huftab = ftell(outfile_huftab) + 1;
	long size_outfile_huff = ftell(outfile_huff) + 1;
	double taxa_compressao = 100 - ((double)size_outfile_huff / size_infile) * 100;

	printf("O arquivo foi compactado com sucesso.\n");
	printf("Tamanho do arquivo de entrada: %d bytes\n", size_infile);
	printf("Tamanho do arquivo comprimido com a tabela: %d bytes\n", size_outfile_huftab);
	printf("Tamanho do arquivo comprimido sem a tabela: %d bytes\n", size_outfile_huff);
	printf("Taxa de compressão: %.2f%\n", taxa_compressao);


	fclose(infile);
	fclose(outfile_huff);
	fclose(outfile_huftab);
	exit(EXIT_SUCCESS);
}
