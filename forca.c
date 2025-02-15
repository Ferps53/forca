#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMITE_DE_PALAVRAS 10
#define TAMANHO_ALFABETO 26
#define ARQUIVO_PALAVRAS "palavras.bin"

typedef struct palavra {
  char string[15];
  int tamanho;
} Palavra;

// Cache de palavras -> Segundo o Mestre Erinaldo isso é mais eficiente que ler
// do disco;
typedef struct palavras {
  Palavra palavras_arquivo[100];
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
      escolha_menu != 4 && escolha_menu != 5 && escolha_menu != 6) {
    printf("Insira uma opção válida\n");
    escolha_menu_principal();
  }
}

void ler_palavras_arquivo() {

  FILE *arquivo = fopen(ARQUIVO_PALAVRAS, "rb");

  if (arquivo == NULL) {
    arquivo = fopen(ARQUIVO_PALAVRAS, "wb");

    if (arquivo == NULL) {
      printf("Falha ao ler arquivo ;(\n");
      exit(1);
    }

    fclose(arquivo);
    ler_palavras_arquivo();
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
    printf("Em questão de palavras, não temos palavras\n");
  }

  for (int i = 0; i < palavras.quantidade_atual; i++) {
    Palavra palavra = palavras.palavras_arquivo[i];

    palavra.string[0] = toupper(palavra.string[0]);

    printf("%d. %s\n", i + 1, palavra.string);
  }
}

int nova_palavra_usuario(Palavra *palavra) {
  char *string = ler_palavra_usuario();

  if (string == NULL) {
    return 0;
  }
  *palavra = criar_nova_palavra(string);

  free(string);
  string = NULL;

  if (palavra->tamanho < 5) {
    printf("A palavra deve ter no mínimo 5 letras para ser cadastrada\n");
    return 0;
  }

  return 1;
}

void salvar_buffer_palavras_arquivo() {

  // Limpa o arquivo para adicionar as palavras da forma certa;
  // Append não funcionou aqui
  FILE *arquivo = fopen(ARQUIVO_PALAVRAS, "wb");

  if (arquivo == NULL) {
    printf("Falha ao abrir arquivo\n");
    exit(1);
  }

  fwrite(&palavras, sizeof(Palavras), 1, arquivo);

  fclose(arquivo);
  arquivo = NULL;
}

void adicionar_palavra() {
  Palavra palavra;

  if (nova_palavra_usuario(&palavra) == 0) {
    return;
  }

  // Adiciona no final do vetor do struct
  palavras.palavras_arquivo[palavras.quantidade_atual] = palavra;
  palavras.quantidade_atual++;

  salvar_buffer_palavras_arquivo();
}

void atualizar_palavra() {

  int id = 0;
  exibir_palavras();
  printf("Selecione o código de uma palavra que deseje alterar: ");
  scanf(" %d", &id);
  printf("\n");

  if (id == 0 || id > palavras.quantidade_atual) {
    printf("O código inserido está inválido, tente novamente\n");
    atualizar_palavra();
    return;
  }

  Palavra palavra;
  if (nova_palavra_usuario(&palavra) == 0) {
    return;
  }

  palavras.palavras_arquivo[id - 1] = palavra;
  salvar_buffer_palavras_arquivo();
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
  printf("Digite uma palavra, com no mínimo 5 letras: ");
  // Evita espaços nas palavras do jogo
  scanf(" %s", palavra);
  return palavra;
}

int gerar_numero_da_palavra_aleatoria() {
  srand(time(NULL));

  int numero_aleatorio = rand() % palavras.quantidade_atual;

  printf("%d\n", numero_aleatorio);
  printf("Quantidade atual de palavras: %d\n", palavras.quantidade_atual);
  return numero_aleatorio;
}

Palavra definir_palavra_aleatoria() {
  int numero_da_palavra = gerar_numero_da_palavra_aleatoria();
  Palavra palavra_secreta = palavras.palavras_arquivo[numero_da_palavra];
  printf("%s\n", palavra_secreta.string);
  // Palavra aleatória definida
  return palavra_secreta;
}

// Inicializa o vetor de letras usadas preenchido com '\0' para facilitar a
// validação
void inicializar_vetor_letras_usadas(char letras_usadas[TAMANHO_ALFABETO]) {

  for (int i = 0; i < TAMANHO_ALFABETO; i++) {
    letras_usadas[i] = '\0';
  }
}

int valida_se_letra_foi_usada(char *letra,
                              char letras_usadas[TAMANHO_ALFABETO]) {

  if (letras_usadas[0] == '\0') {
    return 0;
  }

  for (int i = 0; letras_usadas[i] != '\0'; i++) {

    // Sempre faço a comparação com letras minusculas
    *letra = tolower(*letra);
    const char letra_usada = tolower(letras_usadas[i]);

    if (letra_usada == *letra) {
      return 1;
    }
  }

  return 0;
}

// Utiliza recursividade para calcular se teve acertos
int validar_acertos(char palpite, char palavra_secreta[15], unsigned int n) {

  palpite = tolower(palpite);

  if (n == strlen(palavra_secreta)) {
    return 0;
  }

  if (palpite == tolower(palavra_secreta[n])) {
    return 1;
  }

  return validar_acertos(palpite, palavra_secreta, n + 1);
}

// Preenche os palpites corretos no vetor palpite_palavra
void preencher_palpites(char palpite, char *palpite_palavra,
                        char *palavra_secreta, int tamanho_palavra) {

  palpite = tolower(palpite);

  for (int i = 0; i < tamanho_palavra; i++) {

    const char letra_secreta = tolower(palavra_secreta[i]);
    if (letra_secreta == palpite) {

      if (i == 0) {
        // Faz a primeira letra ficar maiuscula
        palpite_palavra[i] = toupper(palpite);
      } else {

        palpite_palavra[i] = letra_secreta;
      }
    }
  }
}

// A strcmp n funcionava com palavras com 5 de tamanho,
// por isso essa função foi feita
int jagura_cmp(char *s1, char *s2, int tamanho_da_palavra_secreta) {

  for (int i = 0; s2[i] != '\0'; i++) {

    if (i > tamanho_da_palavra_secreta) {

      printf("Algo deu errado!\n");
      printf("for passou do tamanho_da_palavra_secreta\n");
      exit(1);
    }

    if (s1[i] != s2[i]) {
      return 0;
    }
  }

  return 1;
}

void o_jogo() { // Sim, você perdeu.
                // Nãããão, eu perdi o jogoooooo!
  if (palavras.quantidade_atual < LIMITE_DE_PALAVRAS) {
    printf("São necessárias no mínimo %d palavras para poder jogar\n",
           LIMITE_DE_PALAVRAS);
    printf("No momento faltam: %d\n",
           LIMITE_DE_PALAVRAS - palavras.quantidade_atual);
    return;
  }

  Palavra palavra_secreta = definir_palavra_aleatoria();

  printf("Essa palavra eh a passada pro jogo: %s\n", palavra_secreta.string);

  const int tamanho_da_palavra = palavra_secreta.tamanho;
  char palpite_palavra[tamanho_da_palavra + 1];

  for (int i = 0; i < tamanho_da_palavra; i++) {
    palpite_palavra[i] = '_';
  }
  palpite_palavra[tamanho_da_palavra + 1] = '\0';

  printf("Hora de Adivinhar. Você tem 6 tentativas...\n");
  printf("%s\n", palpite_palavra);

  char letras_usadas[26];
  int contador_letras_usadas = 0;
  inicializar_vetor_letras_usadas(letras_usadas);

  if (letras_usadas[0] == '\0') {
    printf("Letras Usadas: Nenhuma letra usada ainda... \n");
  }

  int contador_de_tentativas = 6;
  int pontuacao = 0;
  int acertos_consecutivos = 0;
  while (contador_de_tentativas > 0) {
    char palpite_letra;
    printf("Advinhe uma letra da palavra: ");
    scanf(" %c", &palpite_letra);

    if (valida_se_letra_foi_usada(&palpite_letra, letras_usadas)) {
      printf("A letra %c já foi utilizada tente novamente\n", palpite_letra);
      continue;
    }

    letras_usadas[contador_letras_usadas] = palpite_letra;
    contador_letras_usadas++;

    printf("%d letras foram usadas\n", contador_letras_usadas);

    int acertou = validar_acertos(palpite_letra, palavra_secreta.string, 0);

    if (acertou) {
      acertos_consecutivos++;
      printf("%d\n", acertos_consecutivos);
      preencher_palpites(palpite_letra, palpite_palavra, palavra_secreta.string,
                         tamanho_da_palavra);
      printf("Muito bem!\n");
      pontuacao = (pontuacao + 10) * acertos_consecutivos;
    } else {
      acertos_consecutivos = 0;
      contador_de_tentativas--;
      pontuacao = pontuacao - 5;
      printf("Você ainda tem %d tentativas...\n", contador_de_tentativas);
    }

    printf("Você ainda tem %d tentativas...\n", contador_de_tentativas);

    if (jagura_cmp(palpite_palavra, palavra_secreta.string,
                   palavra_secreta.tamanho)) {
      printf("Meus parabéns! Você ganhou!\n");
      if (contador_de_tentativas == 6) {
        printf("Uau! Você conseguiu adivinhar a palavra sem errar nenhuma "
               "letra!\n");
        pontuacao = pontuacao + 300;
        printf("Pontuação Final: %d Pontos.\nObrigado por jogar!\n", pontuacao);
        return;
      } else {
        printf("Pontuação Final: %d Pontos.\n Obrigado por jogar!\n\n",
               pontuacao);
        return;
      }
    }
    printf("%s\n", palpite_palavra);
    printf("Letras usadas: ");
    for (int i = 0; i < contador_letras_usadas; i++) {
      printf("%c ", letras_usadas[i]);
    }
    printf("\n");
  }
  printf("Você Perdeu! Acabaram suas tentativas!\n");
  printf("A palavra era: %s.\n", palavra_secreta.string);
  printf("Pontuação Final:%d\nObrigado por jogar!\n\n", pontuacao);
}

int main() {

  setlocale(LC_ALL, "Portuguese");
  ler_palavras_arquivo();
  printf("Olá! Por favor selecione uma opção:\n");
  int escolha_menu = 0;

  while (escolha_menu != 6) {
    escolha_menu = escolha_menu_principal();
    switch (escolha_menu) {

    case 1: {
      o_jogo();
      break;
    }

    case 2: {
      exibir_palavras();
      break;
    }

    case 3: {
      adicionar_palavra();
      break;
    }

    case 4: {
      atualizar_palavra();
      break;
    }

    case 5:
      break;
    }
  }
  printf("Saindo... Até mais!\n");
  exit(0);
}
