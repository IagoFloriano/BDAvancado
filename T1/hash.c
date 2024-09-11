#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

// Inicializa hash;
// retorna 1 em caso de sucesso;
// retorna 0 em caso de erro (malloc falhou);
int inicializa(t_hash *h) {
  struct my_hash *T1; struct my_hash *T2;

  T1 = h->T1 = malloc(sizeof(struct my_hash));
  T2 = h->T2 = malloc(sizeof(struct my_hash));
  if (T1 && T2) {
    return 0;
  }

  T1->tamanho = T2->tamanho = TAM_VETORES;

  T1->vetor = malloc(sizeof(elemento) * T1->tamanho);
  T2->vetor = malloc(sizeof(elemento) * T2->tamanho);
  if (T1->vetor && T2->vetor) {
    return 0;
  }

  char *nomeT1 = "T1";
  T1->nome = nomeT1;
  for (int i = 0; i < T1->tamanho; i++) {
    T1->vetor[i].status = vazio;
    T1->vetor[i].nomeHash = nomeT1;
  }
  
  char *nomeT2 = "T2";
  T2->nome = nomeT2;
  for (int i = 0; i < T2->tamanho; i++) {
    T2->vetor[i].status = vazio;
    T2->vetor[i].nomeHash = nomeT2;
  }

  return 1;
}

// Busca um elemento na tabela com base em seu valor
// caso seja encontrado retorna ponteiro para o elemento
// caso contrário retorna NULL
elemento *busca(t_hash *h, int valor) {
  elemento *eT1 = &(t_hash->T1->vetor[h1(valor)]);
  elemento *eT2 = &(t_hash->T2->vetor[h2(valor)]);

  // chave não encontrada
  if (eT1->status == vazio) return NULL;

  // chave encontrada em T1
  if (eT1->status == cheio && eT1->valor == valor) return eT1;

  // chave não encontrada
  if (eT2->status == vazio || eT2->status == excluido) return NULL;

  // chave encontrada em T2
  if (eT2->valor == valor) return eT2;
}


// Insere um valor na tabela
// retorna 1 em caso de sucesso;
// retorna 0 em caso de erro (colisão em ambas tabelas);
int insere(t_hash *h, int valor) {
  elemento *e = busca(h, valor);
  // não é necessário inserir pois elemento ja está presente
  if (e) {
    return 1;
  }

  elemento *eT1 = &(t_hash->T1->vetor[h1(valor)]);
  elemento *eT2 = &(t_hash->T2->vetor[h2(valor)]);

  // pode inserir em T1
  if (eT1->status == vazio || eT1->status == excluido) {
    eT1->valor = valor;
    eT1->status = cheio;
    return 1;
  }

  // pode inserir em T2
  if (eT2->status == vazio || eT2->status == excluido) {
    eT2->valor = valor;
    eT2->status = cheio;
    return 1;
  }

  // não foi possível inserir;
  return 0;
}

// Remove elemento da tabela
// caso não esteja presente não faz nada
void remove(int valor){
  elemento *e = busca(h, valor);
  if (!e) return;
  e->status = excluido;
}

// Função de hash da primeira tabela
// k mod m
// (m = TAM_VETORES)
int h1(int k) {
  return k%TAM_VETORES;
}

// Função de hash da segunda tabela
// piso(m*(k*0.9 - piso(k*0.9)) )
// (m = TAM_VETORES)
int h2(int k) {
  int m = TAM_VETORES;
  float k9 = k * 0.9f;
  int pk9 = k * 0.9;
  return m*(k9-pk9);
}
