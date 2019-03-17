Enzo Maruffa Moreira - GRR20171626
Lucas Sampaio Franco - GRR20166836

# Estrutura de dados
 - Lista encadeada de vértices (grafo->vertices)
 - Cada vértice tem uma lista de vizinhos (grafo->vertices->vertices_conectados)


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
