#include "config.h"
#include <stdio.h>
#include <malloc.h>
#include "logic.h"



int loadGame(struct game* game) {
	FILE* file = fopen(SAVE_PATH, "rb");
	if (!file)
		return 1;

	int flag = fread(game, sizeof(game), 1, file);
	for (int i = 0; i < BOARD_SIZE; i++) {
		flag += fread(game->board[i], sizeof(int) * 8 + sizeof(int*), 1, file);
	}

	fclose(file);
	return flag!=9;

}
int saveGame(struct game* game) {
	FILE* file = fopen(SAVE_PATH, "wb");
	if (!file)
		return 1;
	int flag = fwrite(game, sizeof(game), 1, file);
	for (int i = 0; i < BOARD_SIZE; i++) {
		flag += fwrite(game->board[i], sizeof(int) * 8 + sizeof(int*), 1, file);
	}
	fclose(file);
	return flag!=9;
}