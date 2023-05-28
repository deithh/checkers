#include "logic.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"




/*RULES
must kill if possible
back kill not allowed
king moves diagonal freely (back kills allowed), if kill stop at next to killed field
*/


struct game* makeGame(int size) {
    if (size < 4 || size >32 || size % 2 == 1) {
        return NULL;
    }

	struct game* game = malloc(sizeof(size) + size * size * sizeof(int));

	if (!game) {
		printf_s("malloc failed");
		return NULL;
	}

	game->toMove = 0;
    game->size = size;
    int** arr = (int**)malloc(size * sizeof(int*));

    if (!arr) {
        printf_s("malloc failed");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = (int*)malloc(size * sizeof(int));

        if (!arr[i]) {
            printf_s("malloc failed");
            return NULL;
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            arr[i][j] = -1;
        }
    }
	game->board = arr;

	return game;
}

void destructGame(struct game* game) {
    for (int i = 0; i < game->size; i++) {
        free(game->board[i]);
    }
    free(game);
}

void initPawns(struct game* game) {
    int fill = (game->size - 2) / 2;
    for (int i = 0; i < fill; i++) {
        for (int j = (i + 1) % 2; j < game->size; j+=2) {
            game->board[i][j] = field_bpawn;
        }
    }
    for (int i = fill+2; i < game->size; i++) {
        for (int j = (i+1) % 2; j < game->size; j+=2) {
            game->board[i][j] = field_wpawn;
        }
    }
}

void debug(struct game* game) {
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            printf("%d ", game->board[i][j]);
        }
        printf("\n");
    }

}



int move(int from, int to, struct game* game) {
    int fi, fj, ti, tj;
    fi = from / 10;
    fj = from % 10;
    ti = to / 10;
    tj = to % 10;


    game->board[ti][tj] = game->board[fi][fj];
    game->board[fi][fj] = field_empty;
    int i, j, temp = preField(fi, fj, ti, tj);
    i = temp / 10;
    j = temp % 10;
    game->board[i][j] = field_empty;

    
        
    promote(ti, tj, game);

    if ((i != fi || j != fj) && nextKillPoss(ti, tj, game)) {
        game->toMove = (game->toMove + 1) % 2;
        return 1;
    }
    game->toMove = (game->toMove + 1) % 2;
    return 0;
}


int mapfield(int x, int y, struct game* game) {
    if (x >= 80 && x <= 720 && y >= 80 && y <= 720) {
        int i = (y / 80 - 1);
        int j = x / 80 - 1;
        return i * 10 + j;
    }
    return -1;
}
int isEmpty(int field, struct game* game) {
    int fi = field / 10;
    int fj = field % 10;
    if (game->board[fi][fj] == field_empty)
        return 1;
    return 0;
}
int isEnemy(int fi, int fj, struct game* game) {
    if (game->toMove == field_wpawn) {
        if (game->board[fi][fj] == field_bpawn || game->board[fi][fj] == field_bking)
            return 1;
    }
    else {
        if (game->board[fi][fj] == field_wpawn || game->board[fi][fj] == field_wking)
            return 1;
    }
    return 0;
}
int isAlly(int fi, int fj, struct game* game) {
    if (game->toMove == field_wpawn) {
        if (game->board[fi][fj] == field_wpawn || game->board[fi][fj] == field_wking)
            return 1;
    }
    else {
        if (game->board[fi][fj] == field_bpawn || game->board[fi][fj] == field_bking)
            return 1;
    }
    return 0;
}
int preField(int fi, int fj, int ti, int tj) {

    int i = -1, j = -1;
    if (ti > fi)
        i = ti - 1;
    else
        i = ti + 1;

    if (tj < fj)
        j = tj + 1;
    else
        j = tj - 1;

    return i * 10 + j;

}

int legal(int from, int to, struct game* game) {
    int fi, fj, ti, tj;
    int killFlag;
    fi = from / 10;
    fj = from % 10;
    ti = to / 10;
    tj = to % 10;
    if (fi == ti && fj == tj)
        return 0;
    killFlag = anyKillPoss(game);
    switch (game->board[fi][fj]) {
    case field_wpawn:
    case field_bpawn:
        return legalPawn(fi, fj, ti, tj,killFlag, game);
        break;
    case field_wking:
    case field_bking:
        return legalKing(fi, fj, ti, tj, killFlag, game);
        break;
    }

    return 0;

}
int legalPawn(int fi, int fj, int ti, int tj,int killFlag, struct game* game) {
    if (game->toMove != game->board[fi][fj])
        return 0;

    int color = game->toMove == field_wpawn ? 1 : -1;

    if (ti - fi == (-1 * color) && abs(tj - fj) == 1 && !killFlag) {
        if (isEmpty(ti * 10 + tj, game))
            return 1;
    }

    return legalKillP(fi, fj, ti, tj, game);
}
int legalKing(int fi, int fj, int ti, int tj, int killFlag, struct game* game) {
    if (game->toMove+2 != game->board[fi][fj])
        return 0;
    int pre, i=ti, j=tj;

    if (!isEmpty(ti * 10 + tj, game))
        return 0;

    while (!killFlag) {
        pre = preField(fi, fj, i, j);
        i = pre / 10;
        j = pre % 10;
        if (i == fi && j == fj)
            return 1;
        if (!isEmpty(i * 10 + j, game))
            break;
    }

    return legalKillK(fi, fj, ti, tj, game);

}
int legalKillK(int fi, int fj, int ti, int tj, struct game* game) {
    if (!isEmpty(ti * 10 + tj, game))
        return 0;
    int pre = preField(fi, fj, ti, tj);
    int i = pre / 10;
    int j = pre % 10;
    
    if (!isEnemy(i, j, game))
        return 0;

    while (1) {
        pre = preField(fi, fj, i, j);
        i = pre / 10;
        j = pre % 10;
        if (i == fi && j == fj)
            return 1;
        if (!isEmpty(i * 10 + j, game))
            break;
    }
    return 0;
}
int legalKillP(int fi, int fj, int ti, int tj, struct game* game) {
    if (ti >= BOARD_SIZE || tj >= BOARD_SIZE | ti < 0 || tj < 0)
        return 0;
    int color = game->toMove == field_wpawn ? 1 : -1;
    if ((ti - fi == (-2 * color)) && (abs(tj - fj) == 2)) {
        int i, j, temp = preField(fi, fj, ti, tj);
        i = temp / 10;
        j = temp % 10;
        if (isEnemy(i, j, game) && isEmpty(ti * 10 + tj, game))
            return 1;
    }
    return 0;

}

int nextKillPoss(int ti, int tj, struct game* game) {
    int pos = 0;
    int i = ti, j = tj;
    switch (game->board[ti][tj]) {
    case field_wpawn:
        pos += legalKillP(ti, tj, ti - 2, tj - 2, game);
        pos += legalKillP(ti, tj, ti - 2, tj + 2, game);
        break;
    case field_bpawn:
        pos += legalKillP(ti, tj, ti + 2, tj - 2, game);
        pos += legalKillP(ti, tj, ti + 2, tj + 2, game);
        break;
    case field_wking:
    case field_bking:
        while (i < BOARD_SIZE && j < BOARD_SIZE) {
            pos += legalKillK(ti, tj, i, j, game);
            i++;
            j++;
        }
        while (i > 0 && j > 0){
            i--;
            j--;
            pos += legalKillK(ti, tj, i, j, game);
        }
        i = ti;
        j = tj;
        while (i < BOARD_SIZE && j > 0) {
            pos += legalKillK(ti, tj, i, j, game);
            i++;
            j--;
        }
        while (--i > 0 && ++j < BOARD_SIZE) {
            i--;
            j++;
            pos += legalKillK(ti, tj, i, j, game);
        }
        break;
    }
    if (pos > 0)
        return 1;
    return 0;
}
int anyKillPoss(struct game* game) {
    int kills = 0;
    for (int i=0; i < BOARD_SIZE; i++) {
        for (int j=0; j < BOARD_SIZE; j++) {
            if (isAlly(i, j, game))
                kills += nextKillPoss(i, j, game);
        }
    }
    if (kills > 0)
        return 1;
    return 0;
}

int winner(struct game* game) {
    int W = 0, B = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == field_wpawn || game->board[i][j] == field_wking)
                W++;
            else if (game->board[i][j] == field_bpawn || game->board[i][j] == field_bking)
                B++;
        }
    }
    if (W == 0)
        return field_bpawn;
    else if (B == 0)
        return field_wpawn;
    return -1;
}
void promote(int fi, int fj, struct game* game) {
    int piece = game->board[fi][fj];
    if (piece == field_wpawn && fi == 0)
        game->board[fi][fj] = field_wking;
    if (piece == field_bpawn && fi == BOARD_SIZE - 1)
        game->board[fi][fj] = field_bking;
}