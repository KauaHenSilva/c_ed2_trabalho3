#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_MATRICULA 6
#define TAMANHO_DISPONIVEL_HASH 150

typedef struct Funcionario
{
  char matricula[TAMANHO_MATRICULA];
  char nome[TAMANHO_MATRICULA];
  char funcao[TAMANHO_MATRICULA];
  float salario;
} Funcionario;

Funcionario gerar_funcionario()
{
  int numero = rand() % 100000;
  Funcionario funcionario;
  funcionario.salario = (float) numero;

  for (int i = 0; i < TAMANHO_MATRICULA; i++, numero /= 10)
  {
    int digito_atual = numero % 10;
    funcionario.matricula[i] = (char) digito_atual + '0';
    funcionario.nome[i] = (char) digito_atual + '0';
    funcionario.funcao[i] = (char) digito_atual + '0';
  }

  funcionario.matricula[TAMANHO_MATRICULA] = '\0';
  funcionario.nome[TAMANHO_MATRICULA] = '\0';
  funcionario.funcao[TAMANHO_MATRICULA] = '\0';

  // Evitar matriculas comecando com 0, motivo: hash_1
  if (funcionario.matricula[0] == '0')
    funcionario.matricula[0] = '1';

  return funcionario;
}

typedef struct stTabelaHash
{
  int hash[TAMANHO_DISPONIVEL_HASH];
  Funcionario funcionario[TAMANHO_DISPONIVEL_HASH];
} TabelaHash;

void inicializar_tabela_hash(TabelaHash *tabela_hash)
{
  for (int index_hash_atual = 0; index_hash_atual < TAMANHO_DISPONIVEL_HASH; index_hash_atual++)
    tabela_hash->hash[index_hash_atual] = -1;
}

int char_to_int(char c)
{
  return (int) c - '0';
}

int hash_1(char matricula[])
{
  char aux[TAMANHO_MATRICULA];

  aux[0] = matricula[TAMANHO_MATRICULA - 2];
  aux[1] = matricula[TAMANHO_MATRICULA - 1];
  for (int index_matricula_velha = 0, idx_matricula_new = 2;
       index_matricula_velha < TAMANHO_MATRICULA - 2;
       index_matricula_velha++, idx_matricula_new++) 
    aux[idx_matricula_new] = matricula[index_matricula_velha];

  int valorInteiro = atoi((char[]){aux[1], aux[3], aux[5]});
  return valorInteiro % TAMANHO_DISPONIVEL_HASH;
}

int hash_2(char matricula[])
{
    int parte1 = (char_to_int(matricula[1 - 1]) * 100) + (char_to_int(matricula[3 - 1]) * 10) + (char_to_int(matricula[6 - 1]));
    int parte2 = (char_to_int(matricula[2 - 1]) * 100) + (char_to_int(matricula[4 - 1]) * 10) + (char_to_int(matricula[5 - 1]));

    int soma_partes = parte1 + parte2;
    return soma_partes % TAMANHO_DISPONIVEL_HASH;
}

int colisao_hash_1(int hash, char matricula[])
{
  char primeiro_caractere[2] = {matricula[0], '\0'};
  return (hash + atoi(primeiro_caractere));
}

int colisao_hash_2(int hash, char matricula[])
{
  (void) matricula;
  return (hash + 7);
}

int efetivar_hash(TabelaHash *tabela_hash, int hash, char matricula[], Funcionario funcionario, int colisao_hash(int, char[]))
{
  int colisao = 0, inseriu = 0;
  int novo_hash = hash;

  while(novo_hash < TAMANHO_DISPONIVEL_HASH && inseriu == 0)
  {
    if (tabela_hash->hash[novo_hash] == -1)
    {
      tabela_hash->hash[novo_hash] = novo_hash;
      tabela_hash->funcionario[novo_hash] = funcionario;
      inseriu = 1;
    }
    else
    {
      colisao++;
      novo_hash = colisao_hash(novo_hash, matricula);
    }
  }

  if (inseriu == 0)
  {
    tabela_hash->hash[hash] = hash;
    tabela_hash->funcionario[hash] = funcionario;
  }

  return colisao;
}

void test_hash(int hash_func(char[]), int colisao_hash(int, char[]))
{
  TabelaHash tabela_hash;
  inicializar_tabela_hash(&tabela_hash);

  int qtd_colisoes = 0;
  clock_t tempo_efetivar_hash = 0;

  for (int criar_1000_funcionarios = 0; criar_1000_funcionarios < 1000; criar_1000_funcionarios++)
  {
    Funcionario funcionario = gerar_funcionario();

    clock_t inicio = clock();
    int hash = hash_func(funcionario.matricula);
    qtd_colisoes += efetivar_hash(&tabela_hash, hash, funcionario.matricula, funcionario, colisao_hash);
    clock_t fim = clock();

    tempo_efetivar_hash += (fim - inicio);
  }

  printf("Tempo para efetivar: %f\n", (double) tempo_efetivar_hash / CLOCKS_PER_SEC);
  printf("Quantidade de colisoes: %d\n", qtd_colisoes);
}

int main()
{
  srand((unsigned) 1);
  printf("Hash 1\n");
  test_hash(hash_1, colisao_hash_1);
  printf("\nHash 2\n");
  test_hash(hash_2, colisao_hash_2);
  return 0;
}