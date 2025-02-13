#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#define LIMITE_DE_PALAVRAS 10
#define ARQUIVO_PALAVRAS "palavras.bin"

typedef struct palavra {
  char string[15];
  int tamanho;
} Palavra;

// Cache de palavras -> Segundo o Mestre Erinaldo isso é mais eficiente que ler
// do disco;
typedef struct palavras {
  Palavra palavras_arquivo[LIMITE_DE_PALAVRAS];
  int quantidade_atual;
} Palavras;

// Permite usar o struct em todo o código -- Global Elite
Palavras palavras;

void verificacao_escolha_menu(int escolha_menu);
int escolha_menu_principal();
void ver_palavras_disponiveis();
char *ler_palavra_usuario();
Palavra criar_nova_palavra(char *string);

int escolha_menu_principal() {
  int escolha_menu;
  printf("1.Novo jogo\n");
  printf("2.Ver palavras\n");
  printf("3.Adicionar palavra\n");
  printf("4.Editar palavras\n");
  printf("5.Deletar palavras\n");
  printf("6.Sair\n");
  scanf("%d", &escolha_menu);
  verificacao_escolha_menu(escolha_menu);
  return escolha_menu;
}

void verificacao_escolha_menu(int escolha_menu) {
  if (escolha_menu != 1 && escolha_menu != 2 && escolha_menu != 3 &&
      escolha_menu != 4 && escolha_menu != 5 && escolha_menu != 6 &&
      escolha_menu != 7) {
    printf("Insira uma opção válida\n");
    escolha_menu_principal();
  }
}

void ler_palavras_arquivo() {

  FILE *arquivo = fopen(ARQUIVO_PALAVRAS, "rb");

  if (arquivo == NULL) {
    printf("Falha ao abrir arquivo\n");
    exit(1);
  }

  Palavras palavras_temp;
  if (fread(&palavras_temp, sizeof(Palavras), 1, arquivo) == 1) {
    palavras = palavras_temp;
  } else {
    printf("Falha ao ler palavras, mantendo ultimo estado\n");
    printf("Ou o arquivo estava vazio  ¯\\_(ツ)_/¯\n\n");
  }

  fclose(arquivo);
}
void exibir_palavras() {

  if (palavras.quantidade_atual <= 0) {
    printf("Em questão de palavras, não temos palavras");
  }

  for (int i = 0; i < palavras.quantidade_atual; i++) {
    Palavra palavra = palavras.palavras_arquivo[i];
    printf("%d. %s\n", i + 1, palavra.string);
  }
}
void escrever_palavras() {
  char *string = ler_palavra_usuario();

  if (string == NULL) {
    return;
  }

  Palavra palavra = criar_nova_palavra(string);

  // Adiciona no final do vetor do struct
  palavras.palavras_arquivo[palavras.quantidade_atual] = palavra;
  palavras.quantidade_atual++;

  // Limpa o arquivo para adicionar as palavras da forma certa;
  // Append não funcionou aqui
  FILE *arquivo = fopen(ARQUIVO_PALAVRAS, "wb");

  if (arquivo == NULL) {
    printf("Falha ao abrir arquivo\n");
    exit(1);
  }

  fwrite(&palavras, sizeof(Palavras), 1, arquivo);

  free(string);
  string = NULL;

  fclose(arquivo);
  arquivo = NULL;
}

int calcular_tamanho_palavra(char *string) {

  int indice = 0;

  while (string[indice] != '\0' && string[indice] != '\n') {
    indice++;

    if (indice >= 15) {
      free(string);
      printf("Overflow de string ao calcular_tamanho_palavra");
      exit(1);
    }
  }

  return indice;
}

Palavra criar_nova_palavra(char *string) {

  int tamanho = calcular_tamanho_palavra(string);

  Palavra palavra;

  // Merito de Matheus Ris
  // A falta de um strcpy me fez ficar 30min tentando resolver um segfault
  //  Palavra palavra = {string, tamanho} -> n funcionava
  strcpy(palavra.string, string);
  palavra.tamanho = tamanho;

  return palavra;
}

char *ler_palavra_usuario() {
  char *palavra = calloc(15, sizeof(char));

  if (palavra == NULL) {
    return NULL;
  }

  // Evita espaços nas palavras do jogo
  scanf(" %s", palavra);
  return palavra;
}

int main() {
  ler_palavras_arquivo();
  setlocale(LC_ALL, "Portuguese");
  printf("Olá! Por favor selecione uma opção:\n");
  int escolha_menu = 0;

  while (escolha_menu != 6) {
    escolha_menu = escolha_menu_principal();
    switch (escolha_menu) {

    case 1:
    case 4:
    case 5:
    case 6:
      break;

    case 3: {
      exibir_palavras();
      break;
    }

    case 2: {
      escrever_palavras();
      break;
    }
    }
  }
  printf("Saindo... Até mais!\n");
  exit(0);
}
