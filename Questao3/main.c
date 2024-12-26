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
  for (int vertice_inicial = 0; vertice_inicial < QUANTIDADE_DE_VERTICES; vertice_inicial++)
  {
    grafo->vertices[vertice_inicial].id = vertice_inicial;
    for (int vertice_final = 0; vertice_final < QUANTIDADE_DE_VERTICES; vertice_final++)
      grafo->arestas[vertice_inicial][vertice_final].confiabilidade = 0;
  }
}

void exibirGrafo(Grafo *grafo)
{
  for (int vertice_inicial = 0; vertice_inicial < QUANTIDADE_DE_VERTICES; vertice_inicial++)
  {
    printf("Vertice %d: \n", grafo->vertices[vertice_inicial].id);
    for (int vertice_final = 0; vertice_final < QUANTIDADE_DE_VERTICES; vertice_final++)
    {
      printf("\tAresta [%d][%d]: " ,vertice_inicial, vertice_final);
      printf("confibilidade %f\n", grafo->arestas[vertice_inicial][vertice_final].confiabilidade);
    }
  }
}

void popularGrafo(Grafo *grafo)
{
  for (int vertice_inicial = 0; vertice_inicial < QUANTIDADE_DE_VERTICES; vertice_inicial++)
  {
    for (int vertice_final = vertice_inicial + 1; vertice_final < QUANTIDADE_DE_VERTICES; vertice_final++)
    {
      double confiabilidade = (double)rand() / RAND_MAX;
      grafo->arestas[vertice_inicial][vertice_final].confiabilidade = confiabilidade;
      grafo->arestas[vertice_final][vertice_inicial].confiabilidade = confiabilidade;
    }
  }
}
void djcastra(int inicio, int fim, Aresta arestas[QUANTIDADE_DE_VERTICES][QUANTIDADE_DE_VERTICES], double *distancias, int *predecessor)
{
  int visitados[QUANTIDADE_DE_VERTICES];

  // Inicializa as distâncias, visitados e predecessores
  for (int vertice_inicial = 0; vertice_inicial < QUANTIDADE_DE_VERTICES; vertice_inicial++)
  {
    distancias[vertice_inicial] = INFINITO_NEGATIVO;
    visitados[vertice_inicial] = 0;
    predecessor[vertice_inicial] = -1;
  }
  distancias[inicio] = 0;

  int vertice_maior_confianca;
  int confiracao_vertice_atual = 0;
  do 
  {
    vertice_maior_confianca = -1;
    for (int vertice_atual = 0; vertice_atual < QUANTIDADE_DE_VERTICES - 1; vertice_atual++)
      if (!visitados[vertice_atual] && (vertice_maior_confianca == -1 || distancias[vertice_atual] > distancias[vertice_maior_confianca]))
        vertice_maior_confianca = vertice_atual;

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
          distancias[v] = distancias[vertice_maior_confianca] + log(confiabilidade);
          predecessor[v] = vertice_maior_confianca;
        }
      }
    }
    confiracao_vertice_atual++;
  } while (confiracao_vertice_atual < QUANTIDADE_DE_VERTICES - 1 && vertice_maior_confianca != -1);
}

void exibirCaminho(int inicio, int fim, int *predecessor)
{
  if (predecessor[fim] == -1)
    printf("Não existe caminho entre %d e %d\n", inicio, fim);
  else
  {
    int caminho[QUANTIDADE_DE_VERTICES];
    int contador = 0;
    int vertice_atual = fim;
    while (vertice_atual != -1)
    {
      caminho[contador] = vertice_atual;
      vertice_atual = predecessor[vertice_atual];
      contador++;
    }

    printf("Caminho entre %d e %d: ", inicio, fim);
    for (int indice = contador - 1; indice >= 0; indice--)
      printf("%d ", caminho[indice]);
    printf("\n");
  }
}

int main()
{
  srand((unsigned) 1);
  // srand((unsigned) 2);

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