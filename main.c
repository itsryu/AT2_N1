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
			printf("Erro ao alocar a memória. Encerrando o programa...\n");
			exit(1);
		} else {
			int numArtists = 0;

			// Atribuindo os valores para a struct Artist;
			readFile(file, artist, &numArtists);

			// Limpando a tela e exibindo o menu;
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

	printf("1. Inserir artista\n2. Remover artista\n3. Editar artista\n4. Buscar artista\n5. Buscar álbum\n6. Exibir informações\n7. Encerrar o programa\n\n");
	printf("Escolha uma opção: ");

	while (scanf("%d", &input) != 1 && input < 1 || input > 7) {
		printf("Opção inválida. Escolha uma opção válida: ");
		while (getchar() != '\n');
	}

	switch (input) {
		case 1: {
			clearScreen();
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

static void showArtists(Artist* artist, int num) {
	for (int i = 0; i < num; i++) {
		printf("Nome: %s\nGênero: %s\nLocal de nascimento: %s\n", artist[i].name, artist[i].gender, artist[i].origin);

		printf("Álbuns:\n");
		for (int j = 0; j < artist[i].numAlbuns; j++) {
			printf("%s\n", artist[i].albuns[j]);
		}

		printf("\n");
	}
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