#pragma once

typedef struct Artist Artist;

static void configEnvironment();
static void clearScreen();
static void readFile(FILE* file, Artist* artist, int* num);