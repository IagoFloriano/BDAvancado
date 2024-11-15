#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAVES 100
#define MAX_LINE_CHARS 20
#define MAX_M 20

typedef struct nodo {
  int id;
  int *chaves;
  int qtdChaves;
  struct nodo *prox;
  struct nodo *prev;
  // finger table
  int *numEntrada;
  struct nodo **nos;
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

  return t;
}

int noValidoFt(int idNo, int idPrev, int entrada) {
  if (idPrev == idNo) return 1;
  if (idNo == entrada) return 1;

  if (idPrev < idNo && entrada < idNo && entrada > idPrev) return 1;
  if (idPrev < idNo) return 0;

  if (entrada > idPrev) return 1;

  if (entrada < idNo) return 1;
  return 0;
}

void atualizaFT(nodo *no, int m) {
  nodo *atual = no->prox;
  for (int i = 0; i < m; i++) {
    no->numEntrada[i] = (no->id + (int)pow(2, i)) % (int)pow(2,m);
    while (!noValidoFt(atual->id, atual->prev->id, no->numEntrada[i])) {
      atual = atual->prox;
    }
    no->nos[i] = atual;
  }
}

void atualizaFTs(dht *t) {
  atualizaFT(t->tree, t->m);
  nodo *atual = t->tree->prox;
  while (atual != t->tree) {
    atualizaFT(atual, t->m);
    atual = atual->prox;
  }
}

void entrar(dht *t, int idNo) {
  // Verifica se m deve ser atualizado
  if (idNo > t->maiorId) {
    t->maiorId = idNo;
    if ( idNo >= pow(2, t->m) ) {
      t->m = log2(t->maiorId) + 1;
    }
  }

  // Inicializa no
  nodo *novoNo;
  novoNo = malloc(sizeof(nodo));
  novoNo->id = idNo;
  novoNo->chaves = malloc(sizeof(int) * MAXCHAVES);
  novoNo->qtdChaves = 0;
  novoNo->numEntrada = malloc(sizeof(int)*MAX_M);
  novoNo->nos = malloc(sizeof(nodo)*MAX_M);

  // Insere o no no anel
  if (t->tree == NULL) {
    t->tree = novoNo->prev = novoNo->prox = novoNo;
    atualizaFTs(t);
    return;
  }
  nodo *noprox, *noprev;
  noprev = t->tree;
  noprox = noprev->prox;
  while (((noprev->id < noprox->id && (idNo > noprox->id || idNo < noprev->id))
        || (noprox->id < noprev->id && (idNo < noprev->id && idNo > noprox->id)))
      && noprev != noprox) {
    noprev = noprox;
    noprox = noprox->prox;
  }
  noprev->prox = novoNo;
  noprox->prev = novoNo;
  novoNo->prox = noprox;
  novoNo->prev = noprev;
  t->qtdNodos++;

  // Atualiza finger tables
  atualizaFTs(t);

  // Pega chaves do nó posterior
  int idMaior;
  for (idMaior = noprox->qtdChaves-1; idMaior >= 0; idMaior--) {
    if (noprox->chaves[idMaior] < idNo)
      break;
  }
  if (idMaior < 0) return;
  novoNo->qtdChaves = idMaior+1;
  //    Copia chaves do prox
  for (int i = 0; i < novoNo->qtdChaves; i++) {
    novoNo->chaves[i] = noprox->chaves[i];
  }
  //    Remove chaves que foram copiadas do prox
  for (int i = 0; i+idMaior+1 < noprox->qtdChaves; i++) {
    noprox->chaves[i] = noprox->chaves[i+idMaior+1];
  }
  noprox->qtdChaves = noprox->qtdChaves - idMaior - 1;

  return;
}

// funcao para qsort
int comp(const void *a_void, const void *b_void) {
  int a = *(int*)a_void;
  int b = *(int*)b_void;

  return b - a;
}

void sair(dht *t, int idNo) {
  if (t->tree == NULL) return;
  // Encontra no
  nodo *noatual = t->tree;
  while (noatual->id != idNo) {
    noatual = noatual->prox;
  }

  // Tira no do anel
  if (t->qtdNodos == 1) {
    inicializa(t);
    free(noatual->chaves);
    free(noatual->numEntrada);
    free(noatual->nos);
    free(noatual);
    return;
  }
  nodo *noprev = noatual->prev;
  nodo *noprox = noatual->prox;
  noprev->prox = noprox;
  noprox->prev = noprev;
  if (noatual == t->tree) {
    t->tree = noprox;
  }
  t->qtdNodos--;

  // Se era o maior da arvore atualiza o m
  if (t->maiorId) {
    // Encontrar novo maior
    while (noprox->id > noprev->id){
      noprox = noprox->prox;
      noprev = noprev->prev;
    }
    t->maiorId = noprev->id;
    t->m = log2(t->maiorId) + 1;
  }

  // Passa chaves para o nó seguinte
  noprox = noatual->prox;
  for (int i = 0; i < noatual->qtdChaves; i++) {
    noprox->chaves[noprox->qtdChaves] = noatual->chaves[i];
    noprox->qtdChaves++;
  }
  qsort(noprox->chaves, noprox->qtdChaves, sizeof(int), comp);

  // Libera memória
  free(noatual->chaves);
  free(noatual->numEntrada);
  free(noatual->nos);
  free(noatual);

  // Atualiza finger tables
  atualizaFTs(t);
  return;
}

int notemchave(nodo *no, int chave) {
  if (chave < no->id && chave >= no->prev->id) return 1;

  if (no->id < no->prev->id && (chave > no->prev->id || chave < no->id)) return 1;
  return 0;
}

void printNodeFT(nodo *no, int m, int timestamp) {
  printf("%d T %d {%d", timestamp, no->id, no->nos[0]->id);
  for (int i = 1; i < m; i++) {
    printf(",%d", no->nos[i]->id);
  }
  printf("}\n");
}

// retorna o no que contem a chave
nodo *lookup(dht *t, int idNo, int chave, int timestamp, int print) {
  if (t->tree == NULL) return NULL;
  // Encontra no correto para começo do lookup
  nodo *noatual = t->tree;
  while (noatual->id != idNo) {
    noatual = noatual->prox;
  }
  //printf("INICIA BUSCA PELA CHAVE %d NO NO %d\n", chave, noatual->id);

  int qtdNos = 1;
  nodo **nosAtingidos = malloc(sizeof(nodo *)*pow(2,t->m));
  nosAtingidos[0] = noatual;

  while (!(notemchave(noatual, chave))) {
    nodo *novono = noatual;
    for (int i = 0; i < t->m; i++) {
      novono = noatual->nos[i];
      //printf("testando se %d sera o novo no\n", novono->id);
      if (notemchave(novono, chave));
      if (i+1 == t->m) break;
      if (novono->id > chave) break;
    }
    nosAtingidos[qtdNos++] = novono;
    noatual = novono;
    //printf("continuando busca no no %d\n", noatual->id);
  }

  if (print) {
    printf("%d L %d {%d", timestamp, chave, nosAtingidos[0]->id);
    for (int i = 1; i < qtdNos; i++) {
      printf(",%d", nosAtingidos[i]->id);
    }
    printf("}\n");
    for (int i = 0; i < qtdNos; i++) {
      printNodeFT(nosAtingidos[i], t->m, timestamp);
    }
  }
  return noatual;
}

void inclusao(dht *t, int idNo, int chave) {
  nodo *no = lookup(t, idNo, chave, 0, 0);
  // Insere a chave
  no->chaves[no->qtdChaves++] = chave;

  // Ordena chaves
  qsort(no->chaves, no->qtdChaves, sizeof(int), comp);
}

int main(){
  char linha[MAX_LINE_CHARS];
  dht hash;
  inicializa(&hash);

  while (fgets(linha, MAX_LINE_CHARS, stdin) != NULL) {
    linha[strcspn(linha, "\n")] = '\0';

    int timestamp = atoi(linha);
    char *spaco = &(strchr(linha, ' ')[1]);

    char comando = spaco[0];
    spaco = &(strchr(spaco, ' ')[1]);

    int idNo = atoi(spaco);
    spaco = &(strchr(spaco, ' ')[1]);

    int chave;
    if (spaco[0] != '-') chave = atoi(spaco);

    switch (comando) {
      case 'E':
        entrar(&hash, idNo);
        break;
      case 'S':
        sair(&hash, idNo);
        break;
      case 'I':
        inclusao(&hash, idNo, chave);
        break;
      case 'L':
        lookup(&hash, idNo, chave, timestamp, 1);
        break;
      default:
        fprintf(stderr, "Comando não reconhecido, será ignorado\n");
    }
  }
  return 0;
}
