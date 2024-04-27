#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
#include <string.h>
#include <locale.h>

#include "artists.h"

int main(void) {
    configEnvironment();

    FILE* file = fopen(FILE_PATH, "r");

    if(file == NULL) {
        printf("Erro ao abrir o arquivo. Encerrando o programa...\n");
        exit(1);
    } else {
        Artist* artist = (Artist*) malloc(LENGTH * sizeof(Artist));

        if(artist == NULL) {
            printf("Erro ao alocar a mem�ria. Encerrando o programa...\n");
            exit(1);
        } else {
            int numArtists = 0;

            // Atribuindo os valores para a struct Artist;
            readFile(file, artist, &numArtists);

            // Ordenando os artistas em ordem alfab�tica;
            quickSort(artist, 0, numArtists - 1);

            // Limpando a tela e exibindo o menu principal;
            clearScreen();
            showMenu(file, artist, &numArtists);

            // Liberando a mem�ria alocada;
            free(artist);
        }
    }

    // Fechando o arquivo;
    fclose(file);

    // Finalizando o programa;
    clearScreen();
    printf("Programa encerrado com sucesso!\n");

    return 0;
}

static void readFile(FILE* file, Artist* artist, int* num) {
    // Atribuindo o nome, g�nero, e local de nascimento para a struct Artist;
    while(fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n", artist[*num].name, artist[*num].gender, artist[*num].origin) == 3) {
        artist[*num].numAlbuns = 0;

        // Atribuindo os albuns para a struct Artist;
        while(fscanf(file, "%[^\n]\n", artist[*num].albuns[artist[*num].numAlbuns]) == 1) {
            // Verificando se o �lbum � o �ltimo da lista;
            if(strstr(artist[*num].albuns[artist[*num].numAlbuns], "==========") != NULL) break;

            // Verificando se o �ltimo char � um espa�o vazio (pois tem espa�os vazios no arquivo artistas.txt):
            if(artist[*num].albuns[artist[*num].numAlbuns][strlen(artist[*num].albuns[artist[*num].numAlbuns]) - 1] == ' ') {
                artist[*num].albuns[artist[*num].numAlbuns][strlen(artist[*num].albuns[artist[*num].numAlbuns]) - 1] = '\0';
            }

            artist[*num].numAlbuns++;
        }

        (*num)++;
    }
}

// Criando uma fun��o para realizar a troca pois a fun��o partition() est� acumulando uma pilha muito grande de bytes;
static void swap(Artist* a, Artist* b) {
    Artist temp = *a;
    *a = *b;
    *b = temp;
}

// Aviso C6262: A fun��o usa '30952' bytes de pilha. Considere mover alguns dados para heap.
static int partition(Artist* artist, int left, int right) {
    Artist pivot = artist[right];
    int i = left - 1;

    for(int j = left; j < right; j++) {
        Artist* A = &artist[j];
        Artist* B = &pivot;

        if(strcmp(A->name, B->name) <= 0) {
            i++;
            swap(&artist[i], &artist[j]);
        }
    }

    swap(&artist[i + 1], &artist[right]);
    return i + 1;
}

static void quickSort(Artist* artist, int left, int right) {
    if(left < right) {
        int p = partition(artist, left, right);

        quickSort(artist, left, p - 1);
        quickSort(artist, p + 1, right);
    }
}

static void showMenu(FILE* file, Artist* artist, int* num) {
    int input = 0;

    printf("---------- Sistema de gerenciamento de Artistas/Bandas ----------\n\n");
    printf("1. Inserir artista/banda\n2. Remover artista/banda\n3. Editar artista/banda\n4. Buscar artista/banda\n5. Buscar �lbum/single\n6. Exibir todas informa��es\n7. Sobre o programa\n8. Salvar e encerrar o programa\n\n");
    printf("Escolha uma op��o: ");

    while(scanf("%d", &input) != 1 && input < 1 || input > 8) {
        printf("Op��o inv�lida. Escolha uma op��o v�lida: ");
        while(getchar() != '\n');
    }

    switch(input) {
        case 1:
        {
            clearScreen();
            addArtist(artist, num);
            quickSort(artist, 0, *num - 1);
            saveFile(file, artist, *num);
            backToMenu(file, artist, num);
            break;
        }
        case 2:
        {
            clearScreen();
            removeArtist(artist, num);
            saveFile(file, artist, *num);
            backToMenu(file, artist, num);
            break;
        }
        case 3:
        {
            clearScreen();
            editArtist(artist, *num);
            quickSort(artist, 0, *num - 1);
            saveFile(file, artist, *num);
            backToMenu(file, artist, num);
            break;
        }
        case 4:
        {
            clearScreen();
            char name[LENGTH] = { "" };

            printf("Nome do artista/banda a ser procurado: ");
            while(scanf(" %[^\n]", name) != 1) {
                printf("Erro ao ler o nome do artista/banda. Tente novamente: ");
                while(getchar() != '\n');
            }

            clearScreen();
            const int index = findArtist(artist, 0, *num - 1, name);

            if(index != -1) {
                printf("Nome: %s\nG�nero: %s\nLocal de nascimento: %s\nN� total de �lbuns/singles: %d\n\n", artist[index].name, artist[index].gender, artist[index].origin, artist[index].numAlbuns);

                printf("�lbuns:\n");
                for(int i = 0; i < artist[index].numAlbuns; i++) {
                    printf("%s\n", artist[index].albuns[i]);
                }

                printf("\n");
            } else {
                printf("Artista n�o encontrado.\n");
            }

            backToMenu(file, artist, num);
            break;
        }
        case 5:
        {
            clearScreen();
			char* name = (char*) malloc(50 * sizeof(char));

			printf("Nome do �lbum/single a ser procurado: ");
            while(scanf(" %[^\n]", name) != 1) {
				printf("Erro ao ler o nome do �lbum/single. Tente novamente: ");
				while(getchar() != '\n');
            }

            clearScreen();
            const FindAlbumResult res = findAlbum(artist, name, *num);

            if(res.index_album != -1) {
                printf("�lbum/single %s do artista/banda %s foi encontrado.\n\n", artist[res.index_artist].albuns[res.index_album], artist[res.index_artist].name);

                printf("Deseja ver mais informa��es do artista/banda?\n");
                printf("1. Sim\n2. N�o\n\n");
                printf("Escolha uma op��o: ");
                while(scanf("%d", &input) != 1 && input < 1 || input > 2) {
                    printf("Op��o inv�lida. Escolha uma op��o v�lida: ");
                    while(getchar() != '\n');
                }

                switch(input) {
                    case 1:
                    {
                        clearScreen();
                        const int index = findArtist(artist, 0, *num - 1, artist[res.index_artist].name);

                        if(index != -1) {
                            printf("Nome: %s\nG�nero: %s\nLocal de nascimento: %s\nN� total de �lbuns/singles: %d\n\n", artist[index].name, artist[index].gender, artist[index].origin, artist[index].numAlbuns);

                            printf("�lbuns:\n");
                            for(int i = 0; i < artist[index].numAlbuns; i++) {
                                printf("%s\n", artist[index].albuns[i]);
                            }

                            printf("\n");
                        } else {
                            printf("Artista n�o encontrado.\n");
                        }

                        break;
                    }
                    case 2:
                    {
                        break;
                    }
                }
            } else {
				printf("�lbum/single n�o encontrado.\n");
            }

            free(name);
            backToMenu(file, artist, num);
            break;
        }
        case 6:
        {
            clearScreen();
            showArtists(artist, *num);
            backToMenu(file, artist, num);
            break;
        }
        case 7:
        {
            clearScreen();
            aboutMe();
            backToMenu(file, artist, num);
            break;
        }
        // Retornando a fun��o main e encerrando o programa devidamente;
        case 8:
        {
            saveFile(file, artist, *num);
            return;
        }
    }
}

static void addArtist(Artist* artist, int* num) {
    if(*num >= LENGTH) {
        printf("Lista de artistas/bandas chegou ao seu limite, remova algum artista para continuar.\n");
        return;
    } else {
        // Obs: nome deve se iniciar em letra mai�scula para ser ordenado corretamente;
        // Poss�vel solu��o: criar uma fun��o para realizar a capitaliza��o da primeira letra do nome do artista;
        printf("Nome do artista/banda: ");
        while(scanf(" %[^\n]", artist[*num].name) != 1) {
            printf("Erro ao ler o nome do artista. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("G�nero musical: ");
        while(scanf(" %[^\n]", artist[*num].gender) != 1) {
            printf("Erro ao ler o g�nero musical. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("Local de nascimento: ");
        while(scanf(" %[^\n]", artist[*num].origin) != 1) {
            printf("Erro ao ler o local de nascimento. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("Quantidade de �lbuns: ");
        while(scanf("%d", &artist[*num].numAlbuns) != 1) {
            printf("Erro ao ler a quantidade de �lbuns. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("\n");
        for(int i = 0; i < artist[*num].numAlbuns; i++) {
            printf("%d� �lbum: ", i + 1);

            while(scanf(" %[^\n]", artist[*num].albuns[i]) != 1) {
                printf("Erro ao ler o �lbum. Tente novamente: ");
                while(getchar() != '\n');
            }
        }

        (*num)++;
    }
}

static void removeArtist(Artist* artist, int* num) {
    char name[LENGTH] = { "" };

    printf("Nome do artista/banda a ser removido: ");
    while(scanf(" %[^\n]", name) != 1) {
        printf("Erro ao ler o nome do artista. Tente novamente: ");
        while(getchar() != '\n');
    }

	const int index = findArtist(artist, 0, *num - 1, name);

    if(index != -1) {
        for(int i = index; i < *num; i++) {
            artist[i] = artist[i + 1];
        }

        (*num)--;

        printf("Artista/banda removido com sucesso.\n");
    } else {
        printf("Artista/banda n�o encontrado na lista. Adicione-o.\n");
    }
}

static void editArtist(Artist* artist, int num) {
    char name[LENGTH] = { "" };

    printf("Nome do artista/banda a ser editado: ");
    while(scanf(" %[^\n]", name) != 1) {
        printf("Erro ao ler o nome do artista. Tente novamente: ");
        while(getchar() != '\n');
    }

	const int index = findArtist(artist, 0, num - 1, name);

    printf("\n");
    if(index != -1) {
        int input = 0;

        printf("Deseja alterar quais informa��es?\n");
        printf("1. Nome\n2. G�nero\n3. Local de nascimento\n4. �lbuns\n5. Todas\n\n");
        printf("Escolha uma op��o: ");

        while(scanf("%d", &input) != 1 && input < 1 || input > 5) {
            printf("Op��o inv�lida. Escolha uma op��o v�lida: ");
            while(getchar() != '\n');
        }

        switch(input) {
            case 1:
            {
                printf("Nome atual: %s\n", artist[index].name);
                printf("Digite o novo nome do artista/banda: ");
                while(scanf(" %[^\n]", artist[index].name) != 1) {
                    printf("Erro ao ler o nome do artista. Tente novamente: ");
                    while(getchar() != '\n');
                }

                break;
            }
            case 2:
            {
                printf("G�nero atual: %s\n", artist[index].gender);
                printf("Digite o novo g�nero musical: ");
                while(scanf(" %[^\n]", artist[index].gender) != 1) {
                    printf("Erro ao ler o g�nero musical. Tente novamente: ");
                    while(getchar() != '\n');
                }

                break;
            }
            case 3:
            {
                printf("Local de nascimento atual: %s\n", artist[index].origin);
                printf("Digite o novo local de nascimento: ");
                while(scanf(" %[^\n]", artist[index].origin) != 1) {
                    printf("Erro ao ler o local de nascimento. Tente novamente: ");
                    while(getchar() != '\n');
                }

                break;
            }
            case 4:
            {
                int input = 0;

                printf("\n");
                printf("1. Alterar �lbum espec�fico\n2. Adicionar �lbum\n3. Remover �lbum espec�fico\n4. Alterar todos os �lbuns\n\n");
                printf("Escolha uma op��o: ");

                while(scanf("%d", &input) != 1 && input < 1 || input > 4) {
                    printf("Op��o inv�lida. Escolha uma op��o v�lida: ");
                    while(getchar() != '\n');
                }

                switch(input) {
                    case 1:
                    {
                        char* name = (char*) malloc(50 * sizeof(char));
						
                        printf("Digite o nome do �lbum a ser alterado: ");
                        while(scanf(" %[^\n]", name) != 1) {
                            printf("Erro ao ler o nome do �lbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        const FindAlbumResult res = findAlbum(artist, name, num);

                        if(res.index_album != -1) {
                            printf("Nome atual: %s\n", artist[index].albuns[res.index_album]);
                            printf("Digite o novo nome do �lbum: ");
                            while(scanf(" %[^\n]", artist[index].albuns[res.index_album]) != 1) {
                                printf("Erro ao ler o nome do �lbum. Tente novamente: ");
                                while(getchar() != '\n');
                            }

                            printf("�lbum alterado com sucesso para '%s'!\n", artist[index].albuns[res.index_album]);
                        } else {
							printf("�lbum n�o encontrado na lista...\n");
						}

                        free(name);
                        break;
                    }
                    case 2:
                    {
                        printf("Digite o novo �lbum: ");
                        while(scanf(" %[^\n]", artist[index].albuns[artist[index].numAlbuns]) != 1) {
                            printf("Erro ao ler o �lbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        artist[index].numAlbuns++;

						printf("�lbum '%s' adicionado com sucesso!\n", artist[index].albuns[artist[index].numAlbuns - 1]);
                        break;
                    }
                    case 3:
                    {
                        char* name = (char*) malloc(50 * sizeof(char));

                        printf("Digite o nome do �lbum a ser removido: ");
                        while(scanf(" %[^\n]", name) != 1) {
                            printf("Erro ao ler o nome do �lbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        const FindAlbumResult res = findAlbum(artist, name, num);

                        if(res.index_album != -1) {
							for(int i = res.index_album; i < artist[index].numAlbuns; i++) {
								strcpy(artist[index].albuns[i], artist[index].albuns[i + 1]);
							}

							printf("�lbum '%s' removido com sucesso!\n", name);
							artist[index].numAlbuns--;
                        } else {
							printf("�lbum n�o encontrado na lista...\n");
                        }

                        free(name);
                        break;
                    }
                    case 4:
                    {
                        printf("Digite a nova quantidade de �lbuns a serem adicionados: ");
                        while(scanf("%d", &artist[index].numAlbuns) != 1) {
                            printf("Erro ao ler a quantidade de �lbuns. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        printf("\n");
                        for(int i = 0; i < artist[index].numAlbuns; i++) {
                            printf("%d� �lbum: ", i + 1);
                        }

                        break;
                    }
                }

                break;
            }
            case 5:
            {
                printf("Digite o novo nome do artista/banda: ");
                while(scanf(" %[^\n]", artist[index].name) != 1) {
                    printf("Erro ao ler o nome do artista. Tente novamente: ");
                    while(getchar() != '\n');
                }

                printf("Digite o novo g�nero musical: ");
                while(scanf(" %[^\n]", artist[index].gender) != 1) {
                    printf("Erro ao ler o g�nero musical. Tente novamente: ");
                    while(getchar() != '\n');
                }

                printf("Digite o novo local de nascimento: ");
                while(scanf(" %[^\n]", artist[index].origin) != 1) {
                    printf("Erro ao ler o local de nascimento. Tente novamente: ");
                    while(getchar() != '\n');
                }

                printf("Quantidade de novos �lbuns: ");
                while(scanf("%d", &artist[index].numAlbuns) != 1) {
                    printf("Erro ao ler a quantidade de �lbuns. Tente novamente: ");
                    while(getchar() != '\n');
                }


                printf("\n");
                for(int i = 0; i < artist[index].numAlbuns; i++) {
                    printf("%d� �lbum: ", i + 1);

                    while(scanf(" %[^\n]", artist[index].albuns[i]) != 1) {
                        printf("Erro ao ler o �lbum. Tente novamente: ");
                        while(getchar() != '\n');
                    }
                }

                break;
            }
        }
    } else {
        printf("Artista/banda n�o encontrado na lista...\n");
    }
}

// Solucionado o problema: a fun��o findArtist() n�o estava retornando o valor correto;
// Solu��o: ordenei o vetor de artistas em ordem alfab�tica (utilizei quick sort) e inverti a ordem dos par�metros da fun��o strcmp();
static int findArtist(Artist* artist, int left, int right, char* name) {
    int index = -1;

    while(left <= right) {
        int mid = (left + right) / 2;

        if(strcmp(artist[mid].name, name) == 0) {
            index = mid;
            break;
        } else if(strcmp(artist[mid].name, name) < 0) { // Valores invertidos, primeiro o nome do artista na struct e depois o nome a ser procurado;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return index;
}

static FindAlbumResult findAlbum(Artist* artist, char* name, int num) {
    FindAlbumResult res = { -1, -1 };

    for(int i = 0; i < num; i++) {
        for(int j = 0; j < artist[i].numAlbuns; j++) {
            if(strcmp(name, artist[i].albuns[j]) == 0) {
                res.index_album = j;
                res.index_artist = i;
                break;
            }
        }
    }
 
    return res;
}

static void saveFile(FILE* file, Artist* artist, int num) {
    file = fopen(FILE_PATH, "w");

    if(file == NULL) {
        printf("Erro ao abrir o arquivo. Encerrando o programa...\n");
        exit(1);
    } else {
        for(int i = 0; i < num; i++) {
            fprintf(file, "%s\n%s\n%s\n", artist[i].name, artist[i].gender, artist[i].origin);

            for(int j = 0; j < artist[i].numAlbuns; j++) {
                fprintf(file, "%s\n", artist[i].albuns[j]);
            }

            fprintf(file, "===========\n");
        }

        fclose(file);
    }
}

static void showArtists(Artist* artist, int num) {
    for(int i = 0; i < num; i++) {
        printf("Nome: %s\nG�nero: %s\nLocal de nascimento: %s\nN� total de �lbuns/singles: %d\n\n", artist[i].name, artist[i].gender, artist[i].origin, artist[i].numAlbuns);

        printf("�lbuns:\n");
        for(int j = 0; j < artist[i].numAlbuns; j++) {
            printf("%s\n", artist[i].albuns[j]);
        }

        printf("\n======================\n\n");
    }
}

static void backToMenu(FILE* file, Artist* artist, int* num) {
    system("pause");

    clearScreen();
    showMenu(file, artist, num);
}

static void aboutMe() {
    printf("Programa desenvolvido por: Jo�o Victor\n");
    printf("C�digo-fonte: https://github.com/itsryu/AT2_N1/\n");
	printf("Data de cria��o: 27/04/2024\n");
    printf("Vers�o: 1.0.0\n\n");

    printf("Copyright � 2024 Victor. Todos os direitos reservados.\n\n");
}

static void configEnvironment() {
    setlocale(LC_ALL, "Portuguese");
    system("color 0A");
    system("title Gerenciador de Artistas/Bandas");
}

static void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #elif __linux__
    system("clear");
    #endif
}