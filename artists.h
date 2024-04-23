#pragma once

typedef struct Artist Artist;

static void configEnvironment();
static void clearScreen();
static void readFile(FILE* file, Artist* artist, int* num);
static void showMenu(FILE* file, Artist* artist, int* num);
static void showArtists(Artist* artist, int num);
static void backToMenu(FILE* file, Artist* artist, int* num);
static void addArtist(Artist* artist, int* num);
static void removeArtist(Artist* artist, int* num);
static void saveFile(FILE* file, Artist* artist, int num);