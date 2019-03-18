#include <stdlib.h>
#include <string.h>
#include "grafo.h"

#define STR_SIZE 1024 // Tamanho máximo de string

unsigned long int ultimo_id_vertice = 0; // Váriavel global para contar a quantidade de vértices / o id do último vértice

struct vertice
{
    char *nome;
    unsigned long int id;
    unsigned long int grau;
    char gap[8]; /* padding */
    struct celula_vertice *vertices_conectados;
};

typedef struct celula_vertice
{
    char gap[8]; /* padding */
    struct vertice *vertice;
    struct celula_vertice *anterior;
    struct celula_vertice *prox;
} celula_vertice;

typedef struct grafo_t
{
    char *nome;
    int num_vertices;
    char gap[12]; /* padding */
    struct celula_vertice *vertices;
} grafo_t;

struct grafo
{
    grafo_t *grafo;
};

/****************************************** Prototypes ***********************************************/
grafo criar_grafo(void);
vertice criar_vertice(char *v_nome);
int criar_aresta(vertice u, vertice v);
int adicionar_vertice(grafo g, vertice v);
vertice vertice_existe(grafo g, char *v_nome);
int aresta_existe(vertice u, vertice v);
int conecta_v_em_u(vertice u, vertice v);
celula_vertice *retorna_celula(grafo g, char *v_nome);
int esta_na_vizinhanca(celula_vertice *vizinhanca, vertice v);

/****************************** Funções fora da especificação ****************************************/

// Inicializa a estrutura de dados do grafo
grafo criar_grafo(void)
{
    grafo g = malloc(sizeof(struct grafo));
    g->grafo = malloc(sizeof(struct grafo_t));
    g->grafo->num_vertices = 0;

    return g;
}

// Cria um novo vértice
vertice criar_vertice(char *v_nome)
{
    vertice v = malloc(sizeof(struct vertice));

    v->nome = malloc(strlen(v_nome) + 1);
    strcpy(v->nome, v_nome);

    v->id = ultimo_id_vertice;
    v->grau = 0;
    v->vertices_conectados = malloc(sizeof(struct celula_vertice));

    ultimo_id_vertice += 1;
    return v;
}

// Se o vertice existe, retorna ele
vertice vertice_existe(grafo g, char *v_nome)
{
    if (g->grafo->num_vertices == 0)
        return NULL;

    struct celula_vertice *celula_atual = g->grafo->vertices;

    do
    {
        if (strcmp(celula_atual->vertice->nome, v_nome) == 0)
            return celula_atual->vertice;
        celula_atual = celula_atual->prox;
    } while (celula_atual != NULL);

    return NULL;
}

// Procura a celula_vertice que tenha o nome == v_nome e retorna ela
celula_vertice *retorna_celula(grafo g, char *v_nome)
{
    struct celula_vertice *celula_atual = g->grafo->vertices;
    do
    {
        if (strcmp(celula_atual->vertice->nome, v_nome) == 0)
            return celula_atual;
        celula_atual = celula_atual->prox;
    } while (celula_atual != NULL);

    return NULL;
}

// Verifica se aresta já existe
int aresta_existe(vertice u, vertice v)
{
    if (u->grau == 0 || v->grau == 0)
        return 0;

    struct celula_vertice *celula_atual = v->vertices_conectados;

    do
    {
        if (celula_atual->vertice == u)
            return 1;
        celula_atual = celula_atual->prox;
    } while (celula_atual != NULL);

    return 0;
}

// Adiciona o vertice criado (v) na lista de vértices do grafo
int adicionar_vertice(grafo g, vertice v)
{
    struct celula_vertice *ultima_celula = NULL;

    if (g->grafo->num_vertices == 0)
    {
        g->grafo->vertices = malloc(sizeof(struct celula_vertice));

        ultima_celula = g->grafo->vertices;
    }
    else
    {
        //busca a última célula
        ultima_celula = g->grafo->vertices;
        while (ultima_celula->prox != NULL)
            ultima_celula = ultima_celula->prox;

        ultima_celula->prox = malloc(sizeof(struct celula_vertice));
        ultima_celula->prox->anterior = ultima_celula;
        ultima_celula = ultima_celula->prox;
    }

    ultima_celula->vertice = v;
    ultima_celula->prox = NULL;
    g->grafo->num_vertices++;

    return 0;
}

// Adiciona vertice v na lista de vertices_conectados do vértice u
int conecta_v_em_u(vertice u, vertice v)
{
    struct celula_vertice *celula_vizinho_u = malloc(sizeof(struct celula_vertice));
    if (u->grau > 0)
    {
        struct celula_vertice *primeiro_vizinho_u = u->vertices_conectados;
        celula_vizinho_u->prox = primeiro_vizinho_u;
        primeiro_vizinho_u->anterior = celula_vizinho_u;
    }
    else
    {
        celula_vizinho_u->prox = NULL;
    }

    celula_vizinho_u->anterior = NULL;
    celula_vizinho_u->vertice = v;

    u->vertices_conectados = celula_vizinho_u;

    u->grau += 1;

    return 0;
}

// Cria a aresta, adicionando vertice u nos vizinhos de v e vertice v nos vizinhos de u
int criar_aresta(vertice u, vertice v)
{

    if (aresta_existe(u, v))
        return 0;

    conecta_v_em_u(u, v);
    conecta_v_em_u(v, u);

    return 1;
}

// Verifica se vertice v está na lista de vizinhos dado
int esta_na_vizinhanca(celula_vertice *vizinhanca, vertice v)
{
    celula_vertice *atual = vizinhanca;
    do
    {
        if (strcmp(atual->vertice->nome, v->nome) == 0)
            return 1;
        atual = atual->prox;
    } while (atual != NULL);

    return 0;
}

/****************************** Funções especifícadas ****************************************/

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//         ou
//         0, caso contrário
int destroi_grafo(grafo g)
{
    free(g->grafo->vertices);
    free(g->grafo);
    return 1;
}

//------------------------------------------------------------------------------
// lê um grafo
//
// devolve o grafo lido,
//         ou
//         NULL, em caso de erro
grafo le_grafo(FILE *input)
{
    grafo g = criar_grafo();

    long unsigned int i = 0;
    char str[STR_SIZE];

    char *token;

    char *u_nome;
    char *v_nome;

    vertice u;
    vertice v;

    while (fgets(str, STR_SIZE, input) != NULL)
    {
        i = strlen(str) - 1;

        if (str[i] == '\n')
            str[i] = '\0';

        // printf("String: %s\n", str);

        char tokenizeStr[STR_SIZE];
        strcpy(tokenizeStr, str);
        token = strtok(tokenizeStr, " ");

        int tokens_read = 0;
        while (token != NULL)
        {
            // printf("token: %s\n", token);
            if (tokens_read == 0)
            {
                u_nome = token;
                u = vertice_existe(g, u_nome);
                if (u == NULL)
                {
                    u = criar_vertice(u_nome);
                    adicionar_vertice(g, u);
                }
            }
            else if (tokens_read == 1)
            {
                v_nome = token;
                v = vertice_existe(g, v_nome);

                if (v == NULL)
                {
                    v = criar_vertice(v_nome);
                    adicionar_vertice(g, v);
                }

                criar_aresta(u, v);
            }
            token = strtok(NULL, " ");
            tokens_read++;
        }
    };
    escreve_grafo(stdout, g);

    return g;
}

//------------------------------------------------------------------------------
// escreve o grafo g em output
//
// devolve o grafo escrito,
//         ou
//         NULL, em caso de erro
grafo escreve_grafo(FILE *output, grafo g)
{
    struct celula_vertice *celula_atual = g->grafo->vertices;
    struct celula_vertice *celula_vizinho;
    do
    {
        fprintf(output, "vértice: %s - %ld\n", celula_atual->vertice->nome, celula_atual->vertice->id);

        if (celula_atual->vertice->grau > 0)
        {
            celula_vizinho = celula_atual->vertice->vertices_conectados;
            do
            {
                fprintf(output, "  - vizinho: %s;\n", celula_vizinho->vertice->nome);
                celula_vizinho = celula_vizinho->prox;
            } while (celula_vizinho != NULL);
        }

        celula_atual = celula_atual->prox;
    } while (celula_atual != NULL);

    return g;
}

//------------------------------------------------------------------------------
// devolve o coeficiente de agrupamento de g
// ou seja, o número de tríades fechadas divido pelo
// total de tríades (ou seja, a soma de tríades abertas e fechadas).
double coeficiente_agrupamento_grafo(grafo g)
{
    double triades_fechadas = 0;
    double triades_abertas = 0;
    double triades_totais = 0;


    struct celula_vertice *v = g->grafo->vertices;
    do
    {
        if (v->vertice->grau > 0)
        {              
            if (v->vertice->grau == 2) {
                triades_totais+=1;
            } else if (v->vertice->grau != 1) {
                triades_totais+= (double) v->vertice->grau;
            }
            printf("triades_totais: %f\n", triades_totais);
            struct celula_vertice *u = v->vertice->vertices_conectados;
            do
            {
                if (u->vertice->grau > 0)
                {
                    printf("u: %s\n", u->vertice->nome);
                    struct celula_vertice *w = u->vertice->vertices_conectados;
                    do
                    {
                        if (strcmp(v->vertice->nome, w->vertice->nome) != 0)
                        {
                            if (esta_na_vizinhanca(v->vertice->vertices_conectados, w->vertice)) // triade fechada
                            {
                                if (u->vertice->id > w->vertice->id)
                                {
                                    printf("fechada+1: v: %s, u: %s, w: %s\n", v->vertice->nome, u->vertice->nome, w->vertice->nome);
                                    triades_fechadas += 1;
                                }
                            }
                        }
                        w = w->prox;
                    } while (w != NULL);
                }

                u = u->prox;
            } while (u != NULL);
        }

        v = v->prox;
    } while (v != NULL);

    printf("Aberta: %f; Fechada: %f \n", triades_totais - triades_fechadas, triades_fechadas);

    return (triades_fechadas / (triades_totais));
}