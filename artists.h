#pragma once

#include <stdio.h>

#define LENGTH 100
#define FILE_PATH "./artistas.txt"

typedef struct {
	char name[LENGTH];
	char gender[LENGTH];
	char origin[LENGTH];
	char albuns[LENGTH][LENGTH];
	int numAlbuns;
} Artist;

typedef struct {
	int index_album;
	int index_artist;
} FindAlbumResult;

static void aboutMe();
static void configEnvironment();
static void clearScreen();
static void readFile(FILE* file, Artist* artist, int* num);
static void showMenu(FILE* file, Artist* artist, int* num);
static void showArtists(Artist* artist, int num);
static void backToMenu(FILE* file, Artist* artist, int* num);
static void addArtist(Artist* artist, int* num);
static void removeArtist(Artist* artist, int* num);
static void editArtist(Artist* artist, int num);
static int findArtist(Artist* artist, int left, int right, char* name);
static FindAlbumResult findAlbum(Artist* artist, char* name, int num);
static void saveFile(FILE* file, Artist* artist, int num);
static void swap(Artist* a, Artist* b);
static int partition(Artist* artist, int left, int right);
static void quickSort(Artist* artist, int left, int right);