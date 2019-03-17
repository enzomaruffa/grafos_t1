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

typedef struct grafo_t
{
    char *nome;
    int num_vertices;
    struct celula_vertice *vertices;
} grafo_t;

struct grafo 
{
    grafo_t *grafo;
};


grafo criar_grafo(void);
vertice criar_vertice(char *v_nome);
int vertice_existe(grafo g, char *v_nome);
int adicionar_vertice(grafo g, vertice v);
int criar_aresta(vertice u, vertice v);

grafo criar_grafo(void) 
{
    grafo g = malloc(sizeof(struct grafo));
    g->grafo = malloc(sizeof(struct grafo_t));
    g->grafo->num_vertices = 0;

    return g;
}

vertice criar_vertice(char *v_nome) 
{
    vertice v = malloc(sizeof(struct vertice));

    v->nome = v_nome;
    v->id = ultimo_id_vertice;
    v->grau = 0;
    v->vertices_conectados = malloc(sizeof(struct celula_vertice)); 
    
    ultimo_id_vertice += 1;
    return v;
}

int vertice_existe(grafo g, char *v_nome) {
    if (g->grafo->num_vertices == 0) 
        return 0;

    struct celula_vertice* celula_atual = g->grafo->vertices;

    while(celula_atual->prox != NULL) {
        if (strcmp(celula_atual->vertice->nome, v_nome) == 0)
            return 1;
        celula_atual = celula_atual->prox;
    }

    // verifica última célula
    if (strcmp(celula_atual->vertice->nome, v_nome) == 0) {        
        return 1;
    }

    return 0;
}

int retorna_vertice(grafo g, char *v_nome) {
    struct celula_vertice* celula_atual = g->grafo->vertices;
    while(celula_atual->prox != NULL) {
        if (strcmp(celula_atual->vertice->nome, v_nome) == 0)
            return celula_atual;
        celula_atual = celula_atual->prox;
    }
    // verifica última célula
    if (strcmp(celula_atual->vertice->nome, v_nome) == 0) {        
        return celula_atual;
    }
    return NULL;
}


int aresta_existe(vertice u, vertice v) {
    if (u->grau == 0 || v->grau == 0) 
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
    struct celula_vertice *ultima_celula = NULL;

    if (g->grafo->num_vertices == 0) {
        g->grafo->vertices = malloc(sizeof(struct celula_vertice));
  
        ultima_celula = g->grafo->vertices;
    } else {

        //busca a última célula
        ultima_celula = g->grafo->vertices;
            // printf("ultima cel %s \n", ultima_celula->vertice->nome);
        while (ultima_celula->prox != NULL) {
            ultima_celula = ultima_celula->prox;
        }

        ultima_celula->prox = malloc(sizeof(struct celula_vertice));        
        ultima_celula->prox->anterior = ultima_celula;
        ultima_celula = ultima_celula->prox;
    }

    ultima_celula->vertice = v;
    ultima_celula->prox = NULL;
    g->grafo->num_vertices++;

    return 0;
}

int conecta_v_em_u(vertice u, vertice v) {
    struct celula_vertice *celula_vizinho_u = malloc(sizeof(struct celula_vertice));
    if (u->grau > 0) {
        struct celula_vertice *primeiro_vizinho_u = u->vertices_conectados;
        celula_vizinho_u->prox = primeiro_vizinho_u;
        primeiro_vizinho_u->anterior = celula_vizinho_u;
    } else {
        celula_vizinho_u->prox = NULL;
    }

    celula_vizinho_u->anterior = NULL;
    celula_vizinho_u->vertice = v;

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

        char tokenizeStr[1024];
        strcpy(tokenizeStr, str);
        token = strtok(tokenizeStr, " ");
        printf("token: %s\n", token);

        int tokens_read = 0;
        while (token != NULL) {
            if (tokens_read == 0) {
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
                } else {
                    v = retorna_vertice(g, v_name);
                }

                criar_aresta(u, v);
            }
            token = strtok(NULL, " ");
            tokens_read++;
        }

        fgets(str, 90, stdin);
    };
    escreve_grafo(stdout,g);

    return g;
}

grafo escreve_grafo(FILE *output, grafo g)
{   
    struct celula_vertice* celula_atual = g->grafo->vertices;
    struct celula_vertice* celula_vizinho;
    while(celula_atual->prox != NULL) {
        printf("vértice: %s \n", celula_atual->vertice->nome);
        if (celula_atual->vertice->grau > 0) {
            celula_vizinho = celula_atual->vertice->vertices_conectados;
            while(celula_vizinho->prox != NULL) {
                printf("  - vizinho: %s;\n", celula_vizinho->vertice->nome);
                celula_vizinho = celula_vizinho->prox;
            }
            printf("  - vizinho: %s;\n", celula_vizinho->vertice->nome);
        }
        celula_atual = celula_atual->prox;
    }    
    printf("vértice: %s \n", celula_atual->vertice->nome);
    return g;
}

double coeficiente_agrupamento_grafo(grafo g)
{
    return 2;
}   