/*
    Ficheiro: project1.h
    Autora: Francisca Vicente de Almeida (ist1105901)
    Descricao: Ficheiro que contem as definicoes de todas as funcoes
               e estruturas de dados
*/
#ifndef MAIN_H
#define MAIN_H

/* Bibliotecas necessarias a importar */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* 
    Numero maximo para o nome da carreira e da paragem
    (+1 para o caracter terminal) 
*/
#define MAX_NOME_CARREIRA 20 +1
#define MAX_NOME_PARAGEM 50 +1

/* Numero maximo de elementos nas listas (+1 para o caracter terminal) */
#define MAX_CARREIRAS 200 +1
#define MAX_PARAGENS 10000 +1
#define MAX_LIG 30000 +1

/* Numero maximo de caracteres que podem estar num input */
#define MAX_INPUT BUFSIZ

/* Numero maximo de argumentos para os comandos */
#define MAX_ARGUMENTOS 5

typedef double Reais;

/* Estrutura para as linked lists */
typedef struct Node {
	int id;
	struct Node *next;
    struct Node *prev;
} Node;

/* Estrutura para as carreiras */
typedef struct
{
    char nome[MAX_NOME_CARREIRA];
    int numero_paragens;
    Node *paragens_normal;
    Node *paragens_inverso;
    Reais custo;
    Reais duration;
} Carreira;

/* Estrutura para as paragens */
typedef struct
{
    char nome[MAX_NOME_PARAGEM];
    int numero_carreiras;
    int carreiras[MAX_CARREIRAS];
    Reais latitude;
    Reais longitude;
} Paragem;


void leLinha(char list_of_words[][MAX_INPUT], int *argumentos);
int eh_inverso(char str[]);
Node* inserir_no_inicio(Node *head, int id);
Node* inserir_no_final(Node *tail, int id);
void print_lista_normal(Node * lista);
void print_lista_inverso(Node * lista);

int obter_numero_paragens(int carreira);
int obter_numero_carreiras(int paragem) ;
int paragem_existe(char nome[]);
Paragem obter_paragem_por_id(int id);
int criar_paragem(char nome[], Reais latitude, Reais longitude);
int carreira_existe(char nome[]);
Carreira obter_carreira_por_id(int id);
void criar_carreira(char nome[]);
int carreira_em_paragem(int carreira, int paragem);
void adicionar_paragens_carreira(int carreira, int paragem, int comeco);
void print_interlig(int lista[], int size);
void definir_paragens_carreira(int numero_carreira, int numero_paragem1,
                               int numero_paragem2);
int verificar_lig(char carreira_nome[], char nome_paragem1[],
                  char nome_paragem2[], Reais custo, Reais duracao);
void criar_lig(char nome[], char origem[], char destino[],
               Reais custo, Reais duration);

void processar_comandos();

void processar_paragens();
void processar_carreiras();
void processar_ligacao();
void processar_interligacao();
/*TESTE PRATICO*/
void processar_fim();

#endif