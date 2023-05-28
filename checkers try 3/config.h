#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define scene_menu 0
#define scene_board 1
#define scene_results 2

#define color_dark al_map_rgb(66, 30, 17)
#define color_light al_map_rgb(255, 255, 204)
#define color_font al_map_rgb(255, 255, 204)
#define color_background al_map_rgb(0,0,0)
#define color_white al_map_rgb(255,255,255)

#define field_empty -1
#define field_wpawn 0
#define field_bpawn 1
#define field_wking 2
#define field_bking 3

#define BOARD_SIZE 8
#define SAVE_PATH "save/save.dat"
#define FPS 15
#define FIELD_SIZE 80
#define TEXTURE_PATHS { "textures/whitePawn.png", "textures/blackPawn.png", "textures/whiteKing.png", "textures/blackKing.png"}
#define WINDOW_SIZE 800
#define FONT_PATH "fonts/Kanit-Light.ttf"