#ifndef __HASH__
#define __HASH__

#define TAM_VETORES 11

enum status_pos_vetor {
  vazio = 0,
  cheio,
  excluido,
}

typedef struct elemento {
  char *nomeHash;
  int valor;
  int status;
} elemento;

struct my_hash {
  char *nome;
  int tamanho;
  elemento *vetor;
};

typedef struct t_hash {
  struct my_hash *T1;
  struct my_hash *T2;
} t_hash;

// Inicializa hash;
// retorna 1 em caso de sucesso;
// retorna 0 em caso de erro (malloc falhou);
int inicializa(t_hash *h);

// Busca um elemento na tabela com base em seu valor
// caso seja encontrado retorna ponteiro para o elemento
// caso contrário retorna NULL
elemento *busca(t_hash *h, int valor);

// Insere um valor na tabela
// retorna 1 em caso de sucesso;
// retorna 0 em caso de erro (colisão em ambas tabelas);
int insere(t_hash *h, int valor);

// Remove elemento da tabela
// caso não esteja presente não faz nada
void remove(int valor);

// Função de hash da primeira tabela
// k mod m
// (m = TAM_VETORES)
int h1(int k);

// Função de hash da segunda tabela
// piso(m*(k*0.9 - piso(k*0.9)) )
// (m = TAM_VETORES)
int h2(int k);

void imprimeOrdenado(t_hash *h);

#endif //__HASH__
