#include "display.h"
#include "logic.h"
#include "config.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>


const char* texturePaths[] = TEXTURE_PATHS;

ALLEGRO_BITMAP** loadTextures() {
    ALLEGRO_BITMAP** textures = malloc(4 * sizeof(ALLEGRO_BITMAP*));
    if (!textures) {
        printf_s("malloc failed");
        return NULL;
    }
    for (int i = 0; i < 4; i++) {
        textures[i] = al_load_bitmap(texturePaths[i]);
        if (!textures[i]) {
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(textures[j]);
            }
            free(textures);
            return NULL;
        }
    }
    return textures;
}


void drawBoard(struct game * game, ALLEGRO_BITMAP ** textures, ALLEGRO_FONT * font, int hover) {
    al_clear_to_color(color_background);
    int color = 0;
    for (int y = 0; y < BOARD_SIZE; y += 1) {
        for (int x = 0; x < BOARD_SIZE; x += 1) {
            int X, Y;
            X = (x + 1) * FIELD_SIZE;
            Y = (y + 1) * FIELD_SIZE;
            al_draw_filled_rectangle(X, Y, X + FIELD_SIZE, Y + FIELD_SIZE, (color % 2 == 0) ? color_light : color_dark);
            if(game->board[y][x] != -1)
                al_draw_scaled_bitmap(textures[game->board[y][x]], 0, 0, 200, 200, X, Y, FIELD_SIZE, FIELD_SIZE, 0);
            color += 1;
        }
        color += 1;
    }
    if (game->toMove == 0)
        al_draw_text(font, color_white, FIELD_SIZE, (BOARD_SIZE+1) * FIELD_SIZE, 0, "White to move");
    else if(game -> toMove ==1)
        al_draw_text(font, color_white, FIELD_SIZE, (BOARD_SIZE + 1) * FIELD_SIZE, 0, "Black to move");

    if (hover == 2) {
        al_draw_filled_rectangle(WINDOW_SIZE / 2 - 93, (BOARD_SIZE + 1) * FIELD_SIZE + 7, WINDOW_SIZE / 2 + 93, (BOARD_SIZE + 1) * FIELD_SIZE + 63, color_white);
        al_draw_text(font, color_background, WINDOW_SIZE / 2, (BOARD_SIZE + 1) * FIELD_SIZE + 20, ALLEGRO_ALIGN_CENTRE, "Save game");
    }
    else {
        al_draw_rectangle(WINDOW_SIZE / 2 - 90, (BOARD_SIZE + 1) * FIELD_SIZE + 10, WINDOW_SIZE / 2 + 90, (BOARD_SIZE + 1) * FIELD_SIZE + 60, color_white, 3);
        al_draw_text(font, color_white, WINDOW_SIZE / 2, (BOARD_SIZE + 1) * FIELD_SIZE + 20, ALLEGRO_ALIGN_CENTRE, "Save game");
    }
    al_flip_display();
}



void drawMenu(ALLEGRO_FONT * font, int hover) {
    al_clear_to_color(color_background);
    al_draw_text(font, color_white, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 115, ALLEGRO_ALIGN_CENTRE, "CHECKERS 8 x 8");
    if (hover == 1) {
        al_draw_filled_rectangle(WINDOW_SIZE / 2 - 103, WINDOW_SIZE / 2 - 53, WINDOW_SIZE / 2 + 103, WINDOW_SIZE / 2 + 53, color_white);
        al_draw_text(font, color_background, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 15, ALLEGRO_ALIGN_CENTRE, "START");
    }
    else {
        al_draw_rectangle(WINDOW_SIZE / 2 - 100, WINDOW_SIZE / 2 - 50, WINDOW_SIZE / 2 + 100, WINDOW_SIZE / 2 + 50, color_white, 6);
        al_draw_text(font, color_white, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 15, ALLEGRO_ALIGN_CENTRE, "START");
    }

    if (hover == 2) {
        al_draw_filled_rectangle(WINDOW_SIZE / 2 - 93, WINDOW_SIZE / 2 + 73, WINDOW_SIZE / 2 + 93, WINDOW_SIZE / 2 + 153, color_white);
        al_draw_text(font, color_background, WINDOW_SIZE / 2, WINDOW_SIZE / 2 + 97, ALLEGRO_ALIGN_CENTRE, "Load game");
    }
    else {
        al_draw_rectangle(WINDOW_SIZE / 2 - 90, WINDOW_SIZE / 2 + 70, WINDOW_SIZE / 2 + 90, WINDOW_SIZE / 2 + 150, color_white, 6);
        al_draw_text(font, color_white, WINDOW_SIZE / 2, WINDOW_SIZE / 2 + 97, ALLEGRO_ALIGN_CENTRE, "Load game");
    }
    al_flip_display();
}

void drawResults(ALLEGRO_FONT* font, int win, int hover) {
    char * winner = calloc(5, sizeof(char));
    if (win == field_wpawn)
        winner = "White";
    else
        winner = "Black";

    al_clear_to_color(color_background);

    if (!hover) {
        al_draw_rectangle(WINDOW_SIZE / 2 - 100, WINDOW_SIZE / 2 - 50, WINDOW_SIZE / 2 + 100, WINDOW_SIZE / 2 + 50, color_white, 6);
        al_draw_text(font, color_white, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 25, ALLEGRO_ALIGN_CENTRE, winner);
        al_draw_text(font, color_white, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 5, ALLEGRO_ALIGN_CENTRE, "wins");
    }
    else {
        al_draw_filled_rectangle(WINDOW_SIZE / 2 - 103, WINDOW_SIZE / 2 - 53, WINDOW_SIZE / 2 + 103, WINDOW_SIZE / 2 + 53, color_white);
        al_draw_text(font, color_background, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 25, ALLEGRO_ALIGN_CENTRE, winner);
        al_draw_text(font, color_background, WINDOW_SIZE / 2, WINDOW_SIZE / 2 - 5, ALLEGRO_ALIGN_CENTRE, "wins");
    }
    al_flip_display();
}
