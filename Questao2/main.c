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

//-----------------------------------------------
// Algoritmo de Ford-Moore-Bellman para encontrar o menor caminho
void fordMooreBellman(int inicio, int fim, int matrizAdj[CONFIGURACAO_MAXIMA][CONFIGURACAO_MAXIMA])
{
  int distancia[CONFIGURACAO_MAXIMA], antessesor[CONFIGURACAO_MAXIMA];

  for (int indiceVertice = 0; indiceVertice < CONFIGURACAO_MAXIMA; indiceVertice++)
    antessesor[indiceVertice] = -1;

  for (int verticeAtual = 0; verticeAtual < CONFIGURACAO_MAXIMA; verticeAtual++)
    distancia[verticeAtual] = INFINITO;
  distancia[inicio] = 0;

  int isUpdated = 1;
  for (int i = 0; i < CONFIGURACAO_MAXIMA - 1 && isUpdated; ++i)
  {
    isUpdated = 0;
    for (int j = 0; j < CONFIGURACAO_MAXIMA; ++j)
      if (distancia[j] != INFINITO)
        for (int k = 0; k < CONFIGURACAO_MAXIMA; k++)
          if (matrizAdj[j][k] != 0 && distancia[j] < distancia[k])
          {
            distancia[k] = distancia[j] + matrizAdj[j][k];
            antessesor[k] = j;
            isUpdated = 1;
          }
  }

  if (distancia[fim] == INFINITO)
    printf("Não há caminho entre %d e %d.\n", inicio, fim);
  else
  {
    printf("Caminho mínimo entre %d e %d:\n", inicio, fim);
    int caminho[CONFIGURACAO_MAXIMA];
    int index = 0;

    for (int atual = fim; atual != -1; atual = antessesor[atual])
      caminho[index++] = atual;

    for (int i = index - 1; i >= 0; --i)
      printf("%d ", caminho[i]);
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

  clock_t inicio, fim;
  inicio = clock();
  fordMooreBellman(inicial, final, matrizAdj);
  fim = clock();

  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo do algoritmo de Ford-MMoore-Bellman: %f segundos\n", tempo);
  
  return 0;
}
