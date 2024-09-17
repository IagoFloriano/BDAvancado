#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define MAX_LINE_CHARS 10

int main() {
  char linha[MAX_LINE_CHARS];
  t_hash hash;
  if (!inicializa(&hash)) {
    printf("Houve algum problema ao inicializar a hash, encerrando programa\n");
    return 0;
  }

  while (fgets(linha, MAX_LINE_CHARS, stdin) != NULL) {
    linha[strcspn(linha, "\n")] = '\0';

    char comando = linha[0];
    int chave = atoi(&(linha[2]));
    
    switch (comando) {
      case 'i':
        if (!insere(&hash, chave)) {
          fprintf(stderr, "Colisão em ambas tabelas, chave não será inserida\n");
        }
        break;
      case 'r':
        removeChave(&hash, chave);
        break;
      default:
        fprintf(stderr, "Comando não reconhecido, será ignorado\n");
    }
  }
  imprimeOrdenado(&hash);
  return 0;
}
