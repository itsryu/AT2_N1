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
			// do something
		}
	}

	fclose(file);

	return 0;
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
