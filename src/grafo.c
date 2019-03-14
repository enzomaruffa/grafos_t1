#include <stdlib.h>
#include <string.h>
#include "grafo.h"

struct no_vertice;

struct vertice 
{
    char *nome;
    int id;
    no_vertice *vertices_conectados;
    int grau;
};

typedef struct no_vertice
{
    vertice *anterior;
    vertice *vertice;
    vertice *prox;
} no_vertice;

struct grafo 
{
    char *nome;
    int id;
    no_vertice *vertices;
    int num_vertices;
};

grafo criar_grafo(void);
vertice criar_vertice(char *v_nome);
int vertice_existe(grafo g, char *v_nome);
int adicionar_vertice(grafo g, vertice v);
int criar_aresta(vertice u, vertice v);

grafo criar_grafo(void) 
{
    grafo g = malloc(sizeof(grafo));
    g->vertices = malloc(sizeof(no_vertice)*0);
    g->vertices->vertice = NULL;

    //TODO: verificar pointers

    return g;
}

vertice criar_vertice(char *v_nome) 
{
    vertice v = malloc(sizeof(vertice));
    v->nome = v_nome;

    //TODO: adicionar id

    return v;
}

int destroi_grafo(grafo g) 
{
    return 1;
}

grafo le_grafo(FILE *input) 
{
    grafo g = criar_grafo();

    int i = 0;
    char str[80];

    char *token;

    char *u_name;
    char *v_name;

    vertice u;
    vertice v;

    while (fgets(str, 80, stdin) != NULL) {

        
        i = strlen(str)-1;

        if( str[i] == '\n') 
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
                    adicionar_vertice(u, g);
                }
            } else if (tokens_read == 1) {
                v_name = token; 

                if (!vertice_existe(g, v_name)) {
                    v = criar_vertice(v_name);
                    adicionar_vertice(v, g);
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