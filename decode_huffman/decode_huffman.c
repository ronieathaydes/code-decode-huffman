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

void iniciar_lista (node **lista)
{
	*lista = NULL;
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

	p = *lista;
    if (p == NULL) { // verifica se a lista não está vazia
    	*lista = n;
    }else {
    	while ((p != NULL) && (p->frequencia_relativa < n->frequencia_relativa)) { // procura a posição de inserção do novo nó
    		q = p;
    		p = p->prox;
    	}

    	if (p == *lista) {     // o novo nó será inserido no início da lista
			*lista = n;
			n->prox = p;
    	}else if (p == NULL) { // o novo no será inserido no final da lista
    		q->prox = n;
    		n->prox = NULL;
    	}else {                // o novo nó será inserido no meio da lista
    		q->prox = n;
    		n->prox = p;
    	}
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

void decodificar_arquivo(FILE *infile_huftab, FILE *outfile, node *arvore) {

	char simbolo;

	int count_buffer = 7;
	unsigned char buffer;

	node *p = arvore;
	fread(&buffer, sizeof(char), 1, infile_huftab);

	do {
		if ((p->esq == NULL) && (p->dir == NULL)) {
			simbolo = p->simbolo;
			fwrite(&simbolo, sizeof(char), 1, outfile);
			p = arvore;
		}else {
			if (((BIT << count_buffer) & buffer) == 0) {
				p = p->esq;
			}else {
				p = p->dir;
			}

			count_buffer--;
			if (count_buffer < 0) {
				fread(&buffer, sizeof(char), 1, infile_huftab);
				count_buffer = 7;
			}
		}

	} while (!feof(infile_huftab));
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Parametros inválidos.\n");
		exit (EXIT_FAILURE);
	}

	char *inpath = argv[1];

	FILE *infile_huftab;
	if ((infile_huftab = fopen(argv[1], "rb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	// resgatando nome do arquivo
	char nome_arquivo[TAMANHO_MAX_NOME_ARQUIVO] = "";
	int i;
	for (i = 0; i < TAMANHO_MAX_NOME_ARQUIVO; i++) {
		fread(&nome_arquivo[i], sizeof(char), 1, infile_huftab);
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

	// juntando o path ao nome do arquivo para abrí-lo
	char outfile_path[TAMANHO_MAX_PATH + TAMANHO_MAX_NOME_ARQUIVO] = "";
	strcat(outfile_path, outpath);
	strcat(outfile_path, nome_arquivo);
	FILE *outfile;
	if ((outfile = fopen(outfile_path, "wb")) == NULL) {
		printf("O arquivo não pode ser aberto.\n");
		exit(EXIT_FAILURE);
	}

	int qtd_simbolos = 0, total_simbolos = 0;

	// resgatando total de símbolos e tabela de ocorrências
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

	printf("O arquivo foi descompactado com sucesso.\n");

	fclose(infile_huftab);
	fclose(outfile);
	exit(EXIT_SUCCESS);
}

