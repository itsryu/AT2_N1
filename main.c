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
            printf("Erro ao alocar a memória. Encerrando o programa...\n");
            exit(1);
        } else {
            int numArtists = 0;

            // Atribuindo os valores para a struct Artist;
            readFile(file, artist, &numArtists);

            // Ordenando os artistas em ordem alfabética;
            quickSort(artist, 0, numArtists - 1);

            // Limpando a tela e exibindo o menu principal;
            clearScreen();
            showMenu(file, artist, &numArtists);

            // Liberando a memória alocada;
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
    // Atribuindo o nome, gênero, e local de nascimento para a struct Artist;
    while(fscanf(file, "%[^\n]\n%[^\n]\n%[^\n]\n", artist[*num].name, artist[*num].gender, artist[*num].origin) == 3) {
        artist[*num].numAlbuns = 0;

        // Atribuindo os albuns para a struct Artist;
        while(fscanf(file, "%[^\n]\n", artist[*num].albuns[artist[*num].numAlbuns]) == 1) {
            // Verificando se o álbum é o último da lista;
            if(strstr(artist[*num].albuns[artist[*num].numAlbuns], "==========") != NULL) break;

            // Verificando se o último char é um espaço vazio (pois tem espaços vazios no arquivo artistas.txt):
            if(artist[*num].albuns[artist[*num].numAlbuns][strlen(artist[*num].albuns[artist[*num].numAlbuns]) - 1] == ' ') {
                artist[*num].albuns[artist[*num].numAlbuns][strlen(artist[*num].albuns[artist[*num].numAlbuns]) - 1] = '\0';
            }

            artist[*num].numAlbuns++;
        }

        (*num)++;
    }
}

// Criando uma função para realizar a troca pois a função partition() está acumulando uma pilha muito grande de bytes;
static void swap(Artist* a, Artist* b) {
    Artist temp = *a;
    *a = *b;
    *b = temp;
}

// Aviso C6262: A função usa '30952' bytes de pilha. Considere mover alguns dados para heap.
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
    printf("1. Inserir artista/banda\n2. Remover artista/banda\n3. Editar artista/banda\n4. Buscar artista/banda\n5. Buscar álbum/single\n6. Exibir todas informações\n7. Sobre o programa\n8. Salvar e encerrar o programa\n\n");
    printf("Escolha uma opção: ");

    while(scanf("%d", &input) != 1 && input < 1 || input > 8) {
        printf("Opção inválida. Escolha uma opção válida: ");
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
                printf("Nome: %s\nGênero: %s\nLocal de nascimento: %s\nNº total de álbuns/singles: %d\n\n", artist[index].name, artist[index].gender, artist[index].origin, artist[index].numAlbuns);

                printf("Álbuns:\n");
                for(int i = 0; i < artist[index].numAlbuns; i++) {
                    printf("%s\n", artist[index].albuns[i]);
                }

                printf("\n");
            } else {
                printf("Artista não encontrado.\n");
            }

            backToMenu(file, artist, num);
            break;
        }
        case 5:
        {
            clearScreen();
			char* name = (char*) malloc(50 * sizeof(char));

			printf("Nome do álbum/single a ser procurado: ");
            while(scanf(" %[^\n]", name) != 1) {
				printf("Erro ao ler o nome do álbum/single. Tente novamente: ");
				while(getchar() != '\n');
            }

            clearScreen();
            const FindAlbumResult res = findAlbum(artist, name, *num);

            if(res.index_album != -1) {
                printf("Álbum/single %s do artista/banda %s foi encontrado.\n\n", artist[res.index_artist].albuns[res.index_album], artist[res.index_artist].name);

                printf("Deseja ver mais informações do artista/banda?\n");
                printf("1. Sim\n2. Não\n\n");
                printf("Escolha uma opção: ");
                while(scanf("%d", &input) != 1 && input < 1 || input > 2) {
                    printf("Opção inválida. Escolha uma opção válida: ");
                    while(getchar() != '\n');
                }

                switch(input) {
                    case 1:
                    {
                        clearScreen();
                        const int index = findArtist(artist, 0, *num - 1, artist[res.index_artist].name);

                        if(index != -1) {
                            printf("Nome: %s\nGênero: %s\nLocal de nascimento: %s\nNº total de álbuns/singles: %d\n\n", artist[index].name, artist[index].gender, artist[index].origin, artist[index].numAlbuns);

                            printf("Álbuns:\n");
                            for(int i = 0; i < artist[index].numAlbuns; i++) {
                                printf("%s\n", artist[index].albuns[i]);
                            }

                            printf("\n");
                        } else {
                            printf("Artista não encontrado.\n");
                        }

                        break;
                    }
                    case 2:
                    {
                        break;
                    }
                }
            } else {
				printf("Álbum/single não encontrado.\n");
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
        // Retornando a função main e encerrando o programa devidamente;
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
        // Obs: nome deve se iniciar em letra maiúscula para ser ordenado corretamente;
        // Possível solução: criar uma função para realizar a capitalização da primeira letra do nome do artista;
        printf("Nome do artista/banda: ");
        while(scanf(" %[^\n]", artist[*num].name) != 1) {
            printf("Erro ao ler o nome do artista. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("Gênero musical: ");
        while(scanf(" %[^\n]", artist[*num].gender) != 1) {
            printf("Erro ao ler o gênero musical. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("Local de nascimento: ");
        while(scanf(" %[^\n]", artist[*num].origin) != 1) {
            printf("Erro ao ler o local de nascimento. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("Quantidade de álbuns: ");
        while(scanf("%d", &artist[*num].numAlbuns) != 1) {
            printf("Erro ao ler a quantidade de álbuns. Tente novamente: ");
            while(getchar() != '\n');
        }

        printf("\n");
        for(int i = 0; i < artist[*num].numAlbuns; i++) {
            printf("%dº álbum: ", i + 1);

            while(scanf(" %[^\n]", artist[*num].albuns[i]) != 1) {
                printf("Erro ao ler o álbum. Tente novamente: ");
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
        printf("Artista/banda não encontrado na lista. Adicione-o.\n");
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

        printf("Deseja alterar quais informações?\n");
        printf("1. Nome\n2. Gênero\n3. Local de nascimento\n4. Álbuns\n5. Todas\n\n");
        printf("Escolha uma opção: ");

        while(scanf("%d", &input) != 1 && input < 1 || input > 5) {
            printf("Opção inválida. Escolha uma opção válida: ");
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
                printf("Gênero atual: %s\n", artist[index].gender);
                printf("Digite o novo gênero musical: ");
                while(scanf(" %[^\n]", artist[index].gender) != 1) {
                    printf("Erro ao ler o gênero musical. Tente novamente: ");
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
                printf("1. Alterar álbum específico\n2. Adicionar álbum\n3. Remover álbum específico\n4. Alterar todos os álbuns\n\n");
                printf("Escolha uma opção: ");

                while(scanf("%d", &input) != 1 && input < 1 || input > 4) {
                    printf("Opção inválida. Escolha uma opção válida: ");
                    while(getchar() != '\n');
                }

                switch(input) {
                    case 1:
                    {
                        char* name = (char*) malloc(50 * sizeof(char));
						
                        printf("Digite o nome do álbum a ser alterado: ");
                        while(scanf(" %[^\n]", name) != 1) {
                            printf("Erro ao ler o nome do álbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        const FindAlbumResult res = findAlbum(artist, name, num);

                        if(res.index_album != -1) {
                            printf("Nome atual: %s\n", artist[index].albuns[res.index_album]);
                            printf("Digite o novo nome do álbum: ");
                            while(scanf(" %[^\n]", artist[index].albuns[res.index_album]) != 1) {
                                printf("Erro ao ler o nome do álbum. Tente novamente: ");
                                while(getchar() != '\n');
                            }

                            printf("Álbum alterado com sucesso para '%s'!\n", artist[index].albuns[res.index_album]);
                        } else {
							printf("Álbum não encontrado na lista...\n");
						}

                        free(name);
                        break;
                    }
                    case 2:
                    {
                        printf("Digite o novo álbum: ");
                        while(scanf(" %[^\n]", artist[index].albuns[artist[index].numAlbuns]) != 1) {
                            printf("Erro ao ler o álbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        artist[index].numAlbuns++;

						printf("Álbum '%s' adicionado com sucesso!\n", artist[index].albuns[artist[index].numAlbuns - 1]);
                        break;
                    }
                    case 3:
                    {
                        char* name = (char*) malloc(50 * sizeof(char));

                        printf("Digite o nome do álbum a ser removido: ");
                        while(scanf(" %[^\n]", name) != 1) {
                            printf("Erro ao ler o nome do álbum. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        const FindAlbumResult res = findAlbum(artist, name, num);

                        if(res.index_album != -1) {
							for(int i = res.index_album; i < artist[index].numAlbuns; i++) {
								strcpy(artist[index].albuns[i], artist[index].albuns[i + 1]);
							}

							printf("Álbum '%s' removido com sucesso!\n", name);
							artist[index].numAlbuns--;
                        } else {
							printf("Álbum não encontrado na lista...\n");
                        }

                        free(name);
                        break;
                    }
                    case 4:
                    {
                        printf("Digite a nova quantidade de álbuns a serem adicionados: ");
                        while(scanf("%d", &artist[index].numAlbuns) != 1) {
                            printf("Erro ao ler a quantidade de álbuns. Tente novamente: ");
                            while(getchar() != '\n');
                        }

                        printf("\n");
                        for(int i = 0; i < artist[index].numAlbuns; i++) {
                            printf("%dº álbum: ", i + 1);
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

                printf("Digite o novo gênero musical: ");
                while(scanf(" %[^\n]", artist[index].gender) != 1) {
                    printf("Erro ao ler o gênero musical. Tente novamente: ");
                    while(getchar() != '\n');
                }

                printf("Digite o novo local de nascimento: ");
                while(scanf(" %[^\n]", artist[index].origin) != 1) {
                    printf("Erro ao ler o local de nascimento. Tente novamente: ");
                    while(getchar() != '\n');
                }

                printf("Quantidade de novos álbuns: ");
                while(scanf("%d", &artist[index].numAlbuns) != 1) {
                    printf("Erro ao ler a quantidade de álbuns. Tente novamente: ");
                    while(getchar() != '\n');
                }


                printf("\n");
                for(int i = 0; i < artist[index].numAlbuns; i++) {
                    printf("%dº álbum: ", i + 1);

                    while(scanf(" %[^\n]", artist[index].albuns[i]) != 1) {
                        printf("Erro ao ler o álbum. Tente novamente: ");
                        while(getchar() != '\n');
                    }
                }

                break;
            }
        }
    } else {
        printf("Artista/banda não encontrado na lista...\n");
    }
}

// Solucionado o problema: a função findArtist() não estava retornando o valor correto;
// Solução: ordenei o vetor de artistas em ordem alfabética (utilizei quick sort) e inverti a ordem dos parâmetros da função strcmp();
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
        printf("Nome: %s\nGênero: %s\nLocal de nascimento: %s\nNº total de álbuns/singles: %d\n\n", artist[i].name, artist[i].gender, artist[i].origin, artist[i].numAlbuns);

        printf("Álbuns:\n");
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
    printf("Programa desenvolvido por: João Victor\n");
    printf("Código-fonte: https://github.com/itsryu/AT2_N1/\n");
	printf("Data de criação: 27/04/2024\n");
    printf("Versão: 1.0.0\n\n");

    printf("Copyright © 2024 Victor. Todos os direitos reservados.\n\n");
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