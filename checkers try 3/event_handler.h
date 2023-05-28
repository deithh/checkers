#pragma once

int clickHandler(int x, int y, int scene, struct game * game, int * done);
int hoverHandler(int x, int y, int scene);
void moveHandler(int* active, int* mustMove, struct game* game, int * scene, int x, int y, int * win);
