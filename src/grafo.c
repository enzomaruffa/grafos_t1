#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int ultimo_id_vertice = 0;

struct celula_vertice;

struct vertice 
{
    char *nome;
    int id;
    int grau;
    struct celula_vertice *vertices_conectados;
};

struct celula_vertice
{
    struct vertice *vertice;
    struct celula_vertice *anterior;
    struct celula_vertice *prox;
};

struct grafo 
{
    char *nome;
    int num_vertices;
    struct celula_vertice *vertices;
};

grafo criar_grafo(void);
vertice criar_vertice(char *v_nome);
int vertice_existe(grafo g, char *v_nome);
int adicionar_vertice(grafo g, vertice v);
int criar_aresta(vertice u, vertice v);

grafo criar_grafo(void) 
{
    grafo g = malloc(sizeof(struct grafo));
    g->num_vertices = 0;

    return g;
}

vertice criar_vertice(char *v_nome) 
{
    vertice v = malloc(sizeof(struct vertice));
    v->nome = v_nome;
    v->id = ultimo_id_vertice;
    v->grau = 0;
    
    ultimo_id_vertice += 1;

    return v;
}

int vertice_existe(grafo g, char *v_nome) {
    if (g->num_vertices == 0) 
        return 0;

    struct celula_vertice* celula_atual = g->vertices;

    while(celula_atual->prox != NULL) {
        if (celula_atual->vertice->nome == v_nome) //TODO: mudar == pra comparação de string
            return 1;
        celula_atual = celula_atual->prox;
    }

    // verifica última célula
    if (celula_atual->vertice->nome == v_nome) //TODO: mudar == pra comparação de string
        return 1;

    return 0;
}

int aresta_existe(vertice u, vertice v) {
    if (u->grau == 0 OU v->grau == 0) 
        return 0;

    struct celula_vertice* celula_atual = v->vertices_conectados;

    while(celula_atual->prox != NULL) {
        if (celula_atual->vertice == u) 
            return 1;
        celula_atual = celula_atual->prox;
    }

    // verifica última célula
    if (celula_atual->vertice == u)
        return 1;

    return 0;
}

// vertice não existe ainda, pois foi verificado no loop principal
int adicionar_vertice(grafo g, vertice v) {
    g->num_vertices++;

    struct celula_vertice *ultima_celula = NULL;

    if (g->num_vertices == 0) {
        g->vertices = malloc(sizeof(struct celula_vertice));
        ultima_celula = g->vertices;
    } else {
        //busca a última célula
        ultima_celula = g->vertices;

        while (ultima_celula->prox != NULL) {
            ultima_celula = ultima_celula->prox;
        }

        ultima_celula->prox = malloc(sizeof(struct celula_vertice));
        ultima_celula->prox->anterior = ultima_celula;
        ultima_celula = ultima_celula->prox;
    }

    ultima_celula->vertice = v;
    ultima_celula->prox = NULL;

    return 0;
}

int conecta_v_em_u(vertice u, vertice v) {
    struct celula_vertice *celula_vizinho_u = malloc(sizeof(struct celula_vertice));
    struct celula_vertice *primeiro_vizinho_u = u->vertices_conectados;

    celula_vizinho_u->anterior = NULL;
    celula_vizinho_u->vertice = v;
    celula_vizinho_u->prox = primeiro_vizinho_u;

    primeiro_vizinho_u->anterior = celula_vizinho_u;

    u->vertices_conectados = celula_vizinho_u;

    u->grau += 1;

    return 0;
}

int criar_aresta(vertice u, vertice v) {

    if (aresta_existe(u, v))
        return 0;

    conecta_v_em_u(u, v);
    conecta_v_em_u(v, u);

    return 1;
}

int destroi_grafo(grafo g) 
{
    return 1;
}

grafo le_grafo(FILE *input) 
{
    grafo g = criar_grafo();

    int i = 0;
    char str[1024];

    char *token;

    char *u_name;
    char *v_name;

    vertice u;
    vertice v;

    while (fgets(str, 1024, stdin) != NULL) {
        i = strlen(str)-1;

        if (str[i] == '\n') 
            str[i] = '\0';

        printf("String: %s\n", str);

        token = strtok(str, ' ');
        int tokens_read = 0;

        while (token != NULL) {
            //Segundo token é "--"
            if (tokens_read) {
                u_name = token; 

                if (!vertice_existe(g, u_name)) {
                    u = criar_vertice(u_name);
                    adicionar_vertice(g, u);
                }
            } else if (tokens_read == 1) {
                v_name = token; 

                if (!vertice_existe(g, v_name)) {
                    v = criar_vertice(v_name);
                    adicionar_vertice(g, v);
                }

                criar_aresta(u, v);
            }

            tokens_read++;
        }

        fgets(str, 90, stdin);
    };

    return g;
}

grafo escreve_grafo(FILE *output, grafo g)
{
    return g;
}

double coeficiente_agrupamento_grafo(grafo g)
{
    return 2;
}   