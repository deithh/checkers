#pragma once

const char* texturePaths[];

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct ALLEGRO_FONT ALLEGRO_FONT;

ALLEGRO_BITMAP** loadTextures();

void drawBoard(struct game* game, ALLEGRO_BITMAP** textures, ALLEGRO_FONT * font, int hover);
void drawMenu(ALLEGRO_FONT * font, int hover);
void drawResults(ALLEGRO_FONT* font, int win, int hover);