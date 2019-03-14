#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lê um grafo
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o coeficiente de agrupamento de g
// ou seja, o número de tríades fechadas divido pelo 
// total de tríades (ou seja, a soma de tríades abertas e fechadas).
// 

double coeficiente_agrupamento_grafo(grafo g);

//------------------------------------------------------------------------------
#endif
