#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "artists.h"

int main(void) {
	configEnvironment();

	printf("Hello World!\n");

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