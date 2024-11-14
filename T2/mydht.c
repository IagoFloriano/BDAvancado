#include <stdio.h>
#include <stdlib.h>

#define MAXCHAVES 100

// finger table
typedef struct ft {
  int *numEntrada;
  int *idNo;
} ft;

typedef struct nodo {
  int id;
  int *chaves;
  int qtdChaves;
  ft tabela;
  struct nodo *prox;
  struct nodo *prev;
} nodo;

typedef struct dht {
  nodo *tree;
  int qtdNodos;
  int maiorId;
  int m;
} dht;

dht *inicializa(dht *t) {
  t->tree = NULL;
  t->qtdNodos = 0;
  t->maiorId = -1;
  t->m = 0;

  return t
}

void entrar(int id, dht *t) {
  // Inicializa no
  nodo *novoNo;
  novoNo = malloc(sizeof(nodo));
  novoNo->id = id;
  novoNo->chaves = malloc(sizeof(int) * MAXCHAVES);
  novoNo->qtdChaves = 0;

  // Inicializa finger table do no
  novoNo->tabela.numEntrada = malloc(sizeof(int)*t->m); 
  novoNo->tabela.idNo = malloc(sizeof(int)*t->m); 

  // Insere o no no anel
  return;
}

void sair(int id, dht *t) {
  return;
}

int main(){
  printf("alksdjflaksdjflajk\n");
}
