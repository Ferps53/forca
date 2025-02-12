# Jogo da Forca - UTFPR

## Trabalho prático de jogo da forca

by: Felipe Brostolin Ribeiro (Ferps53) & Matheus Otavio Holdys Ris (OverheatZero) 

1. Introdução

> Este trabalho tem como objetivo a criação de um programa em linguagem C de um jogo da forca.
Em um jogo da forca, o jogador possui 6 chances para adivinhar as letras de uma palavra qualquer.
A palavra deve ser sorteada de um conjunto de 10 palavras distintas com no mínimo 5 letras cada.
Todas as palavras devem estar armazenadas em um mesmo arquivo binário.  
Inicialmente, somente são exibidos traços (underline/underscore) correspondentes a cada letra da palavra. Quando o
jogador tentar adivinhar uma letra, a letra é comparada a cada letra da palavra. Caso a letra
pertença a palavra, todas as posições na qual a letra aparece são reveladas e a pontuação do
jogador aumenta. Caso a letra não pertença a palavra, o número de chances diminui em um e
a pontuação do jogador diminui. Independente da letra pertencer ou não a palavra, a letra que
foi fornecida pelo jogador passa a ser exibida na tela com a lista de letras já utilizadas. O jogo é
finalizado quando o jogador acerta todas as letras da palavra ou quando o jogador não possui mais
chances de adivinhar. Após o término do jogo, é apresentada a pontuação do jogador. Em seguida,
deve ser apresentado um menu com três opções: “Novo Jogo”, “Menu Inicial” e outra “Sair”. A
opção “Novo Jogo” inicia um novo jogo com uma nova palavra, sorteada aleatoriamente. A opção
“Menu Inicial” retorna para o menu inicial do jogo e o apresenta em tela. Por fim, a opção “Sair”
encerra o jogo.

2. Requisitos

- O programa deve seguir as regras anteriores;

- O programa deve apresentar um menu inicial com as seguintes opções:
    - Novo Jogo: para iniciar um novo jogo. Somente permite iniciar o jogo caso tenham no
    mınimo 10 palavras salvas no arquivo;
    - Cadastrar palavra: cadastra uma nova palavra de no m´ınimo 5 letras e a salva no arquivo. Se a palavra já existir, não pode permitir o cadastro dela novamente;
    - Atualizar palavra: atualiza uma das palavras salvas no arquivo;
    - Apagar palavra: seleciona e apaga uma das palavras salvas no arquivo;
    - Mostrar palavras: mostra todas as palavras salvas no arquivo em tela;
    - Sair - encerra o jogo;

3. Desenvolvimento do programa

 - O programa deve ser desenvolvido em linguagem C, utilizando conceitos como funções, ponteiros, alocação de memória, recursão, registros e arquivos;

 - O program deve ser desenvolvido em **duplas**;
