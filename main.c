#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "artists.h"

#define LENGTH 100
#define FILE_PATH "./artists.txt"

typedef struct Artist {
	char name[LENGTH];
	char gender[LENGTH];
	char origin[LENGTH];
	char albuns[LENGTH][LENGTH];
	int numAlbuns;
} Artist;

int main(void) {
	configEnvironment();

	FILE* file = fopen(FILE_PATH, "r");

	if (file == NULL) {
		printf("Erro ao abrir o arquivo. Encerrando o programa...\n");
		exit(1);
	} else {
		Artist* artist = malloc(LENGTH * sizeof(Artist));

		if (artist == NULL) {
			printf("Erro ao alocar a mem�ria. Encerrando o programa...\n");
			exit(1);
		} else {
			int numArtists = 0;

			// Atribuindo os valores para a struct Artist;
			readFile(file, artist, &numArtists);

			// Limpando a tela e exibindo o menu principal;
			clearScreen();
			showMenu(file, artist, &numArtists);
		}
	}

	fclose(file);

	return 0;
}

static void readFile(FILE* file, Artist* artist, int* num) {
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n", artist[*num].name, artist[*num].gender, artist[*num].origin) == 3) {
		artist[*num].numAlbuns = 0;

		while (fscanf(file, "%[^\n]\n", artist[*num].albuns[artist[*num].numAlbuns]) == 1) {
			if (strstr(artist[*num].albuns[artist[*num].numAlbuns], "==========") != NULL) {
				break;
			}

			artist[*num].numAlbuns++;
		}

		(*num)++;
	}
}

static void showMenu(FILE* file, Artist* artist, int* num) {
	int input = 0;

	printf("1. Inserir artista\n2. Remover artista\n3. Editar artista\n4. Buscar artista\n5. Buscar �lbum\n6. Exibir informa��es\n7. Encerrar o programa\n\n");
	printf("Escolha uma op��o: ");

	while (scanf("%d", &input) != 1 && input < 1 || input > 7) {
		printf("Op��o inv�lida. Escolha uma op��o v�lida: ");
		while (getchar() != '\n');
	}

	switch (input) {
		case 1: {
			clearScreen();
			addArtist(artist, num);
			saveFile(file, artist, *num);
			backToMenu(file, artist, num);
			break;
		}
		case 2: {
			clearScreen();
			break;
		}
		case 3: {
			clearScreen();
			break;
		}
		case 4: {
			clearScreen();
			break;
		}
		case 5: {
			clearScreen();
			break;
		}
		case 6: {
			clearScreen();
			showArtists(artist, *num);
			backToMenu(file, artist, num);
			break;
		}
		case 7: {
			fclose(file);
			free(artist);
			exit(0);
			break;
		}
	}

	free(artist);
}

static void addArtist(Artist* artist, int* num) {
	if (*num >= LENGTH) {
		printf("Lista de artistas chegou ao seu limite, remova algum artista para continuar.\n");
		return;
	} else {
		printf("Nome do artista: ");
		while (scanf(" %[^\n]", artist[*num].name) != 1) {
			printf("Erro ao ler o nome do artista. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("G�nero musical: ");
		while (scanf(" %[^\n]", artist[*num].gender) != 1) {
			printf("Erro ao ler o g�nero musical. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Local de nascimento: ");
		while (scanf(" %[^\n]", artist[*num].origin) != 1) {
			printf("Erro ao ler o local de cria��o/nascimento. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Quantidade de �lbuns: ");
		while (scanf("%d", &artist[*num].numAlbuns) != 1) {
			printf("Erro ao ler a quantidade de �lbuns. Tente novamente: ");
			while (getchar() != '\n');
		}

		for (int i = 0; i < artist[*num].numAlbuns; i++) {
			printf("%d� �lbum: ", i + 1);

			while (scanf(" %[^\n]", artist[*num].albuns[i]) != 1) {
				printf("Erro ao ler o �lbum. Tente novamente: ");
				while (getchar() != '\n');
			}
		}

		(*num)++;
	}
}

static void saveFile(FILE* file, Artist* artist, int num) {
	file = fopen(FILE_PATH, "w");

	if (file == NULL) {
		printf("Erro ao abrir o arquivo. Encerrando o programa...\n");
		exit(1);
	} else {
		for (int i = 0; i < num; i++) {
			fprintf(file, "%s\n%s\n%s\n", artist[i].name, artist[i].gender, artist[i].origin);

			for (int j = 0; j < artist[i].numAlbuns; j++) {
				fprintf(file, "%s\n", artist[i].albuns[j]);
			}

			fprintf(file, "===========\n");
		}
	}
}

static void showArtists(Artist* artist, int num) {
	for (int i = 0; i < num; i++) {
		printf("Nome: %s\nG�nero: %s\nLocal de nascimento: %s\n", artist[i].name, artist[i].gender, artist[i].origin);

		printf("�lbuns:\n");
		for (int j = 0; j < artist[i].numAlbuns; j++) {
			printf("%s\n", artist[i].albuns[j]);
		}

		printf("\n");
	}
}

static void backToMenu(FILE* file, Artist* artist, int* num) {
	system("pause");

	clearScreen();
	showMenu(file, artist, num);
}

static void configEnvironment() {
	setlocale(LC_ALL, "Portuguese");
	system("color 0A");
}

static void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#elif __linux__
		system("clear");
	#endif
}