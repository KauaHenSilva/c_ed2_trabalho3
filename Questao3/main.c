#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define QUANTIDADE_DE_VERTICES 3
#define INFINITO_NEGATIVO INT_MIN

typedef struct stAresta
{
  double confiabilidade;
} Aresta;

typedef struct stVertice
{
  int id;
} Vertice;

typedef struct stGrafo
{
  Vertice vertices[QUANTIDADE_DE_VERTICES];
  Aresta arestas[QUANTIDADE_DE_VERTICES][QUANTIDADE_DE_VERTICES];
} Grafo;

void inicializarGrafo(Grafo *grafo)
{
  for (int i = 0; i < QUANTIDADE_DE_VERTICES; i++)
  {
    grafo->vertices[i].id = i;
    for (int j = 0; j < QUANTIDADE_DE_VERTICES; j++)
      grafo->arestas[i][j].confiabilidade = 0;
  }
}

void exibirGrafo(Grafo *grafo)
{
  for (int i = 0; i < QUANTIDADE_DE_VERTICES; i++)
  {
    printf("Vertice %d: \n", grafo->vertices[i].id);
    for (int j = 0; j < QUANTIDADE_DE_VERTICES; j++)
      printf("\t Aresta [%d][%d]: confibilidade %f\n", i, j, grafo->arestas[i][j].confiabilidade);
  }
}

void popularGrafo(Grafo *grafo)
{
  for (int i = 0; i < QUANTIDADE_DE_VERTICES; i++)
    for (int j = 0; j < QUANTIDADE_DE_VERTICES; j++)
      grafo->arestas[i][j].confiabilidade = (double)rand() / RAND_MAX;
}

void djcastra(int inicio, int fim, Aresta arestas[QUANTIDADE_DE_VERTICES][QUANTIDADE_DE_VERTICES], double *distancias, int *predecessor)
{
  int visitados[QUANTIDADE_DE_VERTICES];

  // Inicializa as distâncias, visitados e predecessores
  for (int i = 0; i < QUANTIDADE_DE_VERTICES; i++)
  {
    distancias[i] = INFINITO_NEGATIVO;
    visitados[i] = 0;
    predecessor[i] = -1;
  }
  distancias[inicio] = 0;


  int vertice_maior_confianca; // inicia com 0 para iniciar o loop
  int x = 0;
  do 
  {
    vertice_maior_confianca = -1;
    for (int i = 0; i < QUANTIDADE_DE_VERTICES - 1; i++)
      if (!visitados[i] && (vertice_maior_confianca == -1 || distancias[i] > distancias[vertice_maior_confianca]))
        vertice_maior_confianca = i;

    if (!(vertice_maior_confianca == -1 || distancias[vertice_maior_confianca] == INFINITO_NEGATIVO))
    {
      visitados[vertice_maior_confianca] = 1;

      for (int v = 0; v < QUANTIDADE_DE_VERTICES; v++)
      {
        double confiabilidade = arestas[vertice_maior_confianca][v].confiabilidade;

        if (!visitados[v] && confiabilidade >= 0 &&
            distancias[vertice_maior_confianca] != INFINITO_NEGATIVO && 
            distancias[vertice_maior_confianca] + log(confiabilidade) > distancias[v])
        {
          distancias[v] = distancias[vertice_maior_confianca] + log(arestas[vertice_maior_confianca][v].confiabilidade);
          predecessor[v] = vertice_maior_confianca; 
        }
      }
    }
    x++;
  } while (x < QUANTIDADE_DE_VERTICES - 1 && vertice_maior_confianca != -1);
}

void exibirCaminho(int inicio, int fim, int *predecessor)
{
  if (predecessor[fim] == -1)
    printf("Não existe caminho entre %d e %d\n", inicio, fim);
  else
  {
    int caminho[QUANTIDADE_DE_VERTICES];
    int contador = 0;
    int i = fim;
    while (i != -1)
    {
      caminho[contador] = i;
      i = predecessor[i];
      contador++;
    }

    printf("Caminho entre %d e %d: ", inicio, fim);
    for (int x = contador - 1; x >= 0; x--)
      printf("%d ", caminho[x]);
    printf("\n");
  }
}

int main()
{
  srand((unsigned)time(NULL));

  Grafo grafo;
  inicializarGrafo(&grafo);
  popularGrafo(&grafo);
  exibirGrafo(&grafo);

  double distancias[QUANTIDADE_DE_VERTICES];
  int predecessor[QUANTIDADE_DE_VERTICES];

  djcastra(0, 2, grafo.arestas, distancias, predecessor);

  exibirCaminho(0, 2, predecessor);
  return 0;
}