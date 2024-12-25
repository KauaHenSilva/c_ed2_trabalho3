#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define NUM_DISCOS 4                      // Número de discos
#define NUM_PINOS 3                       // Número de pinos
#define CONFIGURACAO_MAXIMA 3 * 3 * 3 * 3 // NUM_PINOS ^ NUM_DISCOS (número total de configurações possíveis)
#define INFINITO INT_MAX

typedef struct
{
  int configuracao[NUM_DISCOS];
} Vertice;

bool movimentoValido(Vertice vertice1, Vertice vertice2)
{
  bool valido = true;

  int diferenca = 0, indiceDiferente = -1;

  for (int indiceConfiguracao = 0; indiceConfiguracao < NUM_DISCOS && diferenca <= 1; ++indiceConfiguracao)
  {
    if (vertice1.configuracao[indiceConfiguracao] != vertice2.configuracao[indiceConfiguracao])
    {
      diferenca++;
      indiceDiferente = indiceConfiguracao;
    }
  }

  if (diferenca > 1)
    valido = false;
  else
  {
    int invalido = 1;
    for (int indiceVerificacao = 0; indiceVerificacao < NUM_DISCOS && invalido; indiceVerificacao++)
      if (indiceVerificacao != indiceDiferente &&
          ((vertice1.configuracao[indiceVerificacao] == vertice1.configuracao[indiceDiferente] && indiceVerificacao < indiceDiferente) ||
           (vertice2.configuracao[indiceVerificacao] == vertice2.configuracao[indiceDiferente] && indiceVerificacao < indiceDiferente)))
        invalido = 0;

    if (!invalido)
      valido = false;
  }

  return (valido);
}

void gerarConfiguracoes(Vertice *grafo, int matrizAdj[][CONFIGURACAO_MAXIMA])
{
  for (int indiceConfiguracao = 0; indiceConfiguracao < CONFIGURACAO_MAXIMA; indiceConfiguracao++)
    for (int indiceVizinho = 0; indiceVizinho < CONFIGURACAO_MAXIMA; indiceVizinho++)
      matrizAdj[indiceConfiguracao][indiceVizinho] = 0;

  int disco, numero_atribuido;
  for (int indiceConfiguracao = 0; indiceConfiguracao < CONFIGURACAO_MAXIMA; indiceConfiguracao++)
  {
    numero_atribuido = indiceConfiguracao;
    for (disco = 0; disco < NUM_DISCOS; disco++)
    {
      grafo[indiceConfiguracao].configuracao[disco] = numero_atribuido % NUM_PINOS + 1;
      numero_atribuido /= NUM_PINOS;
    }
  }

  for (int indiceAtual = 0; indiceAtual < CONFIGURACAO_MAXIMA; indiceAtual++)
    for (int indiceVizinho = 0; indiceVizinho < CONFIGURACAO_MAXIMA; indiceVizinho++)
      if (movimentoValido(grafo[indiceAtual], grafo[indiceVizinho]))
        matrizAdj[indiceAtual][indiceVizinho] = 1;
      else
        matrizAdj[indiceAtual][indiceVizinho] = 0;
}

void djcastra(int inicio, int fim, int matrizAdj[CONFIGURACAO_MAXIMA][CONFIGURACAO_MAXIMA], int *distancias, int *predecessor)
{
  int visitados[CONFIGURACAO_MAXIMA];

  // Inicializa as distâncias, visitados e predecessores
  for (int i = 0; i < CONFIGURACAO_MAXIMA; i++)
  {
    distancias[i] = INFINITO;
    visitados[i] = 0;
    predecessor[i] = -1;
  }
  distancias[inicio] = 0;


  int vertice_menor_distancia; // inicia com 0 para iniciar o loop
  int x = 0;
  do 
  {
    vertice_menor_distancia = -1;

    for (int i = 0; i < CONFIGURACAO_MAXIMA - 1; i++)
      if (!visitados[i] && (vertice_menor_distancia == -1 || distancias[i] < distancias[vertice_menor_distancia]))
        vertice_menor_distancia = i;

    if (!(vertice_menor_distancia == -1 || distancias[vertice_menor_distancia] == INFINITO))
    {
      visitados[vertice_menor_distancia] = 1;

      for (int v = 0; v < CONFIGURACAO_MAXIMA; v++)
      {
        if (!visitados[v] && matrizAdj[vertice_menor_distancia][v] && 
            distancias[vertice_menor_distancia] != INFINITO && 
            distancias[vertice_menor_distancia] + matrizAdj[vertice_menor_distancia][v] < distancias[v])
        {
          distancias[v] = distancias[vertice_menor_distancia] + matrizAdj[vertice_menor_distancia][v];
          predecessor[v] = vertice_menor_distancia; 
        }
      }
    }
    
    x++;
  } while (x < CONFIGURACAO_MAXIMA - 1 && vertice_menor_distancia != -1);
}

void exibir_caminho(int inicio, int fim, int *distancias, int *predecessor)
{
  if (distancias[fim] == INFINITO)
  {
    printf("Não há caminho acessível de %d para %d.\n", inicio, fim);
  }
  else
  {
    // Exibe a menor distância
    printf("Menor caminho de %d para %d: %d\n", inicio, fim, distancias[fim]);

    // Exibe o caminho
    printf("Caminho: ");
    int caminho[CONFIGURACAO_MAXIMA];
    int indice = 0;
    int atual = fim;

    // Reconstrói o caminho a partir do destino
    while (atual != -1)
    {
      caminho[indice++] = atual;  // Armazena o vértice no caminho
      atual = predecessor[atual];  // Vai para o predecessor
    }

    // Exibe o caminho na ordem correta
    for (int i = indice - 1; i >= 0; i--)
    {
      printf("%d", caminho[i]);
      if (i > 0)
        printf(" -> ");
    }
    printf("\n");
  }
}


void exibir_config(Vertice *vetice)
{
  for (int config_atual = 0; config_atual < CONFIGURACAO_MAXIMA; ++config_atual)
  {
    printf("vertex %d: ", config_atual);
    for (int disco_atual = 0; disco_atual < NUM_DISCOS; disco_atual++)
      printf("%d ", vetice[config_atual].configuracao[disco_atual]);
    printf("\n");
  }
}

int main()
{
  Vertice grafo[CONFIGURACAO_MAXIMA];
  int matrizAdj[CONFIGURACAO_MAXIMA][CONFIGURACAO_MAXIMA];

  gerarConfiguracoes(grafo, matrizAdj);
  exibir_config(grafo);

  int inicial, final;
  printf("Digite o índice da configuração inicial (0 a %d): ", CONFIGURACAO_MAXIMA - 1);
  scanf("%d", &inicial);
  printf("Digite o índice da configuração final (0 a %d): ", CONFIGURACAO_MAXIMA - 1);
  scanf("%d", &final);

  int distancias[CONFIGURACAO_MAXIMA];
  int predecessor[CONFIGURACAO_MAXIMA];

  clock_t inicio, fim;
  inicio = clock();
  djcastra(inicial, final, matrizAdj, distancias, predecessor);
  fim = clock();

  exibir_caminho(inicial, final, distancias, predecessor);

  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo do algoritmo de dijkshtra: %f segundos\n", tempo);

  return 0;
}
