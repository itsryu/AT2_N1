#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

			// Limpando a tela e exibindo o menu principal;
			clearScreen();
			showMenu(file, artist, &numArtists);

			// Liberando a memória alocada;
			free(artist);
		}
	}

	// Fechando o arquivo;
	fclose(file);

	clearScreen();
	printf("Programa encerrado com sucesso!\n");

	return 0;
}

static void readFile(FILE* file, Artist* artist, int* num) {
	while (fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n", artist[*num].name, artist[*num].gender, artist[*num].origin) == 3) {
		artist[*num].numAlbuns = 0;

		while (fscanf(file, "%[^\n]\n", artist[*num].albuns[artist[*num].numAlbuns]) == 1) {
			if (strstr(artist[*num].albuns[artist[*num].numAlbuns], "==========") != NULL) break;

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
		addArtist(artist, num);
		saveFile(file, artist, *num);
		backToMenu(file, artist, num);
		break;
	}
	case 2: {
		clearScreen();
		removeArtist(artist, num);
		saveFile(file, artist, *num);
		backToMenu(file, artist, num);
		break;
	}
	case 3: {
		clearScreen();
		editArtist(artist, *num);
		saveFile(file, artist, *num);
		backToMenu(file, artist, num);
		break;
	}
	// to do: Arrumar a problemática da busca binária
	case 4: {
		clearScreen();
		char name[LENGTH] = { "" };

		printf("Nome do artista a ser procurado: ");
		while (scanf(" %[^\n]", name) != 1) {
			printf("Erro ao ler o nome do artista. Tente novamente: ");
			while (getchar() != '\n');
		}

		const int pos = findArtist(artist, 0, *num, name);

		if (pos != -1) {
			printf("Artista encontrado:\n");
			printf("Nome: %s\n", artist[pos].name);
			printf("Gênero: %s\n", artist[pos].gender);
			printf("Local: %s\n", artist[pos].origin);
			printf("Álbuns:\n");
			for (int i = 0; i < artist[pos].numAlbuns; i++) {
				printf("%s\n", artist[pos].albuns[i]);
			}
		} else {
			printf("Artista não encontrado.\n");
		}

		backToMenu(file, artist, num);
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
		// Retornando a função main e encerrando o programa devidamente;
		return;
	}
	}
}

static void addArtist(Artist* artist, int* num) {
	if (*num >= LENGTH) {
		printf("Lista de artistas chegou ao seu limite, remova algum artista para continuar.\n");
		return;
	}
	else {
		printf("Nome do artista: ");
		while (scanf(" %[^\n]", artist[*num].name) != 1) {
			printf("Erro ao ler o nome do artista. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Gênero musical: ");
		while (scanf(" %[^\n]", artist[*num].gender) != 1) {
			printf("Erro ao ler o gênero musical. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Local de nascimento: ");
		while (scanf(" %[^\n]", artist[*num].origin) != 1) {
			printf("Erro ao ler o local de criação/nascimento. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Quantidade de álbuns: ");
		while (scanf("%d", &artist[*num].numAlbuns) != 1) {
			printf("Erro ao ler a quantidade de álbuns. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("\n");
		for (int i = 0; i < artist[*num].numAlbuns; i++) {
			printf("%dº álbum: ", i + 1);

			while (scanf(" %[^\n]", artist[*num].albuns[i]) != 1) {
				printf("Erro ao ler o álbum. Tente novamente: ");
				while (getchar() != '\n');
			}
		}

		(*num)++;
	}
}

static void removeArtist(Artist* artist, int* num) {
	char name[LENGTH] = { "" };
	int index = -1;

	printf("Nome do artista a ser removido: ");
	while (scanf(" %[^\n]", name) != 1) {
		printf("Erro ao ler o nome do artista. Tente novamente: ");
		while (getchar() != '\n');
	}

	for (int i = 0; i < *num; i++) {
		if (strcmp(artist[i].name, name) == 0) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		for (int i = index; i < *num; i++) {
			artist[i] = artist[i + 1];
		}

		(*num)--;

		printf("Artista removido com sucesso.\n");
	}
	else {
		printf("Artista não encontrado na lista. Adicione-o.\n");
	}
}

static void editArtist(Artist* artist, int num) {
	char name[LENGTH] = { "" };
	int index = -1;

	printf("Nome do artista a ser editado: ");
	while (scanf(" %[^\n]", name) != 1) {
		printf("Erro ao ler o nome do artista. Tente novamente: ");
		while (getchar() != '\n');
	}

	for (int i = 0; i < num; i++) {
		if (strcmp(artist[i].name, name) == 0) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		printf("Novo nome do artista: ");
		while (scanf(" %[^\n]", artist[index].name) != 1) {
			printf("Erro ao ler o nome do artista. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Gênero musical: ");
		while (scanf(" %[^\n]", artist[index].gender) != 1) {
			printf("Erro ao ler o gênero musical. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Local de nascimento: ");
		while (scanf(" %[^\n]", artist[index].origin) != 1) {
			printf("Erro ao ler o local de criação/nascimento. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("Quantidade de álbuns: ");
		while (scanf("%d", &artist[index].numAlbuns) != 1) {
			printf("Erro ao ler a quantidade de álbuns. Tente novamente: ");
			while (getchar() != '\n');
		}

		printf("\n");
		for (int i = 0; i < artist[index].numAlbuns; i++) {
			printf("%dº álbum: ", i + 1);

			while (scanf(" %[^\n]", artist[index].albuns[i]) != 1) {
				printf("Erro ao ler o álbum. Tente novamente: ");
				while (getchar() != '\n');
			}
		}
	}
	else {
		printf("Artista não encontrado na lista...\n");
	}
}

// to do: Encontrar a problemática da busca binária;
int findArtist(Artist* artist, int left, int right, char* name) {
	while (left <= right) {
		int mid = (left + right) / 2;

		printf("[%d] - %s\n", mid, artist[mid].name);

		if (strcmp(name, artist[mid].name) == 0) {
			return mid;
		} else if (strcmp(name, artist[mid].name) < 0) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return -1;
}

static void saveFile(FILE* file, Artist* artist, int num) {
	file = fopen(FILE_PATH, "w");

	if (file == NULL) {
		printf("Erro ao abrir o arquivo. Encerrando o programa...\n");
		exit(1);
	}
	else {
		for (int i = 0; i < num; i++) {
			fprintf(file, "%s\n%s\n%s\n", artist[i].name, artist[i].gender, artist[i].origin);

			for (int j = 0; j < artist[i].numAlbuns; j++) {
				fprintf(file, "%s\n", artist[i].albuns[j]);
			}

			fprintf(file, "===========\n");
		}

		fclose(file);
	}
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