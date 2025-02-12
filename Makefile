# O que vai ser compilado, nesse caso forca.c
# Como será feita a compilação
# $(CC) é o compilador salvo nas variáveis de ambiente
# forca.c - o forca -- gera um executável chamado forca 
# -Wall, -Wextra e -pedantic mostram todos os avisos de possíveis falhas do programa
# -std=c99 define que estamos usando a versão C99 da liguagem C
forca: forca.c
	$(CC) forca.c -o forca -Wall -Wextra -pedantic -std=c99
