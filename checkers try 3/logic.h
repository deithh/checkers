#pragma once

struct game {
	int toMove; //indicates which color should move (0 - white, 1 - black)
	int size;
	int** board; //board[i][j] corresponds to field (0 - white pawn, 2 - white king, 1 - black pawn, 3 - black king, -1 - empty)
};

struct game* makeGame(int size);

void initPawns(struct game* game);
void destructGame(struct game* game);
void debug(struct game* game);

int move(int from, int to, struct game* game); //return 1 if killed
int mapfield(int x, int y, struct game* game);
int isEmpty(int field, struct game* game);
int legal(int from, int to, struct game* game);
int legalPawn(int fi, int fj, int ti, int tj, int killFlag, struct game* game);
int legalKing(int fi, int fj, int ti, int tj, int killFlag, struct game* game);
int legalKillK(int fi, int fj, int ti, int tj, struct game* game);
int legalKillP(int fi, int fj, int ti, int tj, struct game* game);
int isEnemy(int fi, int fj, struct game* game);
int preField(int fi, int fj, int ti, int tj);
void promote(int fi, int fj, struct game* game);
int nextKillPoss(int ti, int tj, struct game* game);
int isAlly(int fi, int fj, struct game* game);
int anyKillPoss(struct game* game);
int winner(struct game* game);