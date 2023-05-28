#include "config.h"
#include "io.h"
#include "logic.h"

int clickHandler(int x, int y, int scene, struct game * game, int * done) {
	switch (scene) {

	case scene_menu:
		 
		switch (hoverHandler(x, y, scene_menu)) {
		case 1:
			return scene_board;
			break;
		case 2:
			if (loadGame(game) == 0)
				return scene_board;
			else {
				printf("Load failed\n");
				break;
			}
		}
		break;
	case scene_board:
		switch (hoverHandler(x, y, scene_board)) {
		case 1:
			return scene_board;
			break;
		case 2:
			if (saveGame(game) == 0)
				printf("save succed\n");
			else {
				printf("save failed\n");
				break;
			}
		}
		
		break;
	case scene_results:
		if (hoverHandler(x, y, scene_results))
			*done = 1;
		break;
	}
	return scene;
}

int hoverHandler(int x, int y, int scene) {
	if (scene == scene_menu)
		if (x <= 500 && x >= 300 && y <= 450 && y >= 350)
			return 1;
		else if (x <= 490 && x >= 310 && y >= 470 && y <= 550)
			return 2;

	if (scene == scene_board) {
		if (x >= 80 && x <= 720 && y>=80 && y<=720)
			return 1;
		if (x >= 310 && x <= 490 && y >= 730 && y <= 790)
			return 2;
	}
	if (scene == scene_results) {
		if (x <= 500 && x >= 300 && y <= 450 && y >= 350)
			return 1;
	}
	return 0;
}

void moveHandler(int* active, int * mustMove, struct game* game, int * scene, int x, int y, int * win) {
	if (*active == field_empty && *scene == scene_board) {
		*active = mapfield(x, y, game);
		if (isEmpty(*active, game))
			*active = field_empty;
	}
	else if (*active != field_empty && *scene == scene_board) {
		int field = mapfield(x, y, game);
		if (legal(*active, field, game)) {
			if (*mustMove == -1 || *mustMove == *active) {
				if (move(*active, field, game)) {
					*mustMove = field;
					game->toMove = (game->toMove + 1) % 2;
				}
				else
					*mustMove = -1;
			}
			else {
				printf("Other pawn is active");
			}
		*active = field_empty;
		}
		else
			printf("Illegal move\n");
			*active = field_empty;
	}

	if ((*win = winner(game)) != -1)
		*scene = scene_results;
}


